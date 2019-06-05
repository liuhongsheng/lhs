#include "common/cwinhttp.h"

string ws2s(const wstring& ws)
{
	string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";

	setlocale(LC_ALL, "chs");

	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);
	wcstombs(_Dest, _Source, _Dsize);
	string result = _Dest;
	delete[]_Dest;

	setlocale(LC_ALL, curLocale.c_str());

	return result;
}

wstring s2ws(const string& s)
{
	setlocale(LC_ALL, "chs");

	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest, _Source, _Dsize);
	wstring result = _Dest;
	delete[]_Dest;

	setlocale(LC_ALL, "C");

	return result;
}


protocol::CWinHttp::~CWinHttp()
{
	if (nullptr != m_ReqData.lpReqData) delete[]m_ReqData.lpReqData;
	if (nullptr != m_RespData.lpHeaders) delete[]m_RespData.lpHeaders;
	if (nullptr != m_RespData.lpData) delete[]m_RespData.lpData;

	m_ReqData.lpReqData = nullptr;
	m_RespData.lpData = nullptr;
	m_RespData.lpHeaders = nullptr;
}

int protocol::CWinHttp::Request()
{
	return Request(m_strUrl);
}

int protocol::CWinHttp::Request(const string strUrl)
{
	return Request(strUrl, m_strMethod);
}

int protocol::CWinHttp::Request(const string strUrl, const string strMethod)
{
	URL_ELEMENT UrlElements;
	ResolvUrl(strUrl, UrlElements);

	wstring wstrAgent = s2ws(m_strAgent);
	wstring wstrVerb = s2ws(m_strMethod);
	wstring wstrAcceptType = s2ws(m_strAcceptType);

	LPCWSTR pszAgentW = wstrAgent.c_str();
	LPCWSTR pswzServerName = UrlElements.wstrHostName.empty() ? NULL : UrlElements.wstrHostName.c_str();
	LPCWSTR pwszVerb = wstrVerb.c_str();
	LPCWSTR pwszObjectName = UrlElements.wstrUrlPath.empty() ? NULL : UrlElements.wstrUrlPath.c_str();

	LPCWSTR pwszAcceptTypes = wstrAcceptType.c_str();
	LPCWSTR *ppwszAcceptTypes = &pwszAcceptTypes;

	LPSTR pszData = (LPSTR)m_ReqData.lpReqData;
	DWORD dwReqDataSize = m_ReqData.dwReqDataSize;

	wstring wstrHeaders = CreateHeaders();
	LPCWSTR lpszHeaders = wstrHeaders.empty() ? NULL : wstrHeaders.c_str();
	DWORD dwHeadersLength = (ULONG)-1;

	SetMethod(strMethod);

	return Request(pszAgentW, pswzServerName, pwszVerb, pwszObjectName, ppwszAcceptTypes, pszData, dwReqDataSize, lpszHeaders, dwHeadersLength, &m_RespData);
}

int protocol::CWinHttp::Request(LPCWSTR pszAgentW, LPCWSTR pswzServerName, LPCWSTR pwszVerb, LPCWSTR pwszObjectName,
	LPCWSTR *ppwszAcceptTypes, LPSTR pszData, DWORD dwReqDataSize, LPCWSTR lpszHeaders, DWORD dwHeadersLength, LPRESPONSE lpResponse)
{
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer = NULL;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(pszAgentW,
		WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect(hSession, pswzServerName, INTERNET_DEFAULT_HTTP_PORT, 0);

	// Create an HTTP request handle.
	if (hConnect)
	{
		hRequest = WinHttpOpenRequest(hConnect, pwszVerb, pwszObjectName,
			L"HTTP/1.1", WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
	}

	// Add a request header.
	if (hRequest && NULL != lpszHeaders && 0 != dwHeadersLength)
	{
		bResults = WinHttpAddRequestHeaders(hRequest, lpszHeaders, dwHeadersLength, WINHTTP_ADDREQ_FLAG_ADD);
		if (!bResults)
			printf("Error %u in WinHttpAddRequestHeaders.\n", GetLastError());
	}

	if (0 == wcscmp(L"POST", pwszVerb) && 0 != dwReqDataSize)
	{
		// Send a request.
		DWORD dwBytesWritten = 0;

		if (hRequest)
		{
			bResults = WinHttpSendRequest(hRequest,
				WINHTTP_NO_ADDITIONAL_HEADERS, 0,
				WINHTTP_NO_REQUEST_DATA, 0, dwReqDataSize, 0);
		}
		else
		{
			printf("Error %u in WinHttpOpenRequest.\n", GetLastError());
		}

		// Write data to the server.
		if (bResults)
			bResults = WinHttpWriteData(hRequest, pszData, dwReqDataSize, &dwBytesWritten);

		if (!bResults)
		{
			printf("Error %u in WinHttpWriteData.\n", GetLastError());
		}
	}
	else
	{
		// Send a request.
		if (hRequest)
		{
			bResults = WinHttpSendRequest(hRequest,
				WINHTTP_NO_ADDITIONAL_HEADERS, 0,
				WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
		}
		else
		{
			printf("Error %u in WinHttpOpenRequest.\n", GetLastError());
		}
	}

	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);

	if (!bResults)
		printf("Error %u in WinHttpReceiveResponse.\n", GetLastError());

	// Keep checking for data until there is nothing left.
	if (bResults)
	{
		do
		{
			// Check for available data.
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
				printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());

			if (0 == dwSize) break;

			// Allocate space for the buffer.
			pszOutBuffer = new char[dwSize + 1];
			if (!pszOutBuffer)
			{
				printf("Out of memory\n");
				dwSize = 0;
			}
			else
			{
				// Read the data.
				ZeroMemory(pszOutBuffer, dwSize + 1);

				if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
					printf("Error %u in WinHttpReadData.\n", GetLastError());
				else
				{
					if (nullptr != lpResponse->lpData)
					{
						delete[]lpResponse->lpData;
						lpResponse->dwDataSize = 0;
					}

					lpResponse->lpData = pszOutBuffer;
					lpResponse->dwDataSize = dwDownloaded;
				}
			}
		} while (dwSize > 0);
	}

	// First, use WinHttpQueryHeaders to obtain the size of the buffer.
	if (bResults)
	{
		WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF,
			WINHTTP_HEADER_NAME_BY_INDEX, NULL,
			&dwSize, WINHTTP_NO_HEADER_INDEX);

		// Allocate memory for the buffer.
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			// Now, use WinHttpQueryHeaders to retrieve the header.
			LPVOID lpOutBuffer = new WCHAR[dwSize / sizeof(WCHAR)];;
			ZeroMemory(lpOutBuffer, dwSize);
			bResults = WinHttpQueryHeaders(hRequest,
				WINHTTP_QUERY_RAW_HEADERS_CRLF,
				WINHTTP_HEADER_NAME_BY_INDEX,
				lpOutBuffer, &dwSize,
				WINHTTP_NO_HEADER_INDEX);

			//wchar_t* to char*
			char *lpHeaders = new char[dwSize + 1];
			ZeroMemory(lpHeaders, dwSize);
			size_t iReturnValue = 0;
			wcstombs_s(&iReturnValue, lpHeaders, size_t(dwSize), (LPWSTR)lpOutBuffer, size_t(dwSize));

			if (nullptr != lpResponse->lpHeaders)
			{
				delete[]lpResponse->lpHeaders;
				lpResponse->dwHeaersSize = 0;
			}

			lpResponse->lpHeaders = lpHeaders;
			lpResponse->dwHeaersSize = dwSize;

			delete[] lpOutBuffer;
		}
	}

	// Report any errors.
	if (!bResults)
		printf("Error %d has occurred.\n", GetLastError());

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

	return 0;
}

VOID protocol::CWinHttp::SetReqData(PVOID lpReqData, DWORD dwReqDataSize)
{
	if (nullptr != m_ReqData.lpReqData)
	{
		delete[]m_ReqData.lpReqData;
		m_ReqData.dwReqDataSize = 0;
	}

	m_ReqData.dwReqDataSize = dwReqDataSize;
	m_ReqData.lpReqData = new char[dwReqDataSize + 1];
	ZeroMemory(m_ReqData.lpReqData, dwReqDataSize + 1);
	memcpy(m_ReqData.lpReqData, lpReqData, dwReqDataSize);
}

const protocol::URL_ELEMENT & protocol::CWinHttp::ResolvUrl(string strUrl, URL_ELEMENT &UrlElements)
{
	URL_COMPONENTS urlComp;

	// Initialize the URL_COMPONENTS structure.
	ZeroMemory(&urlComp, sizeof(urlComp));
	urlComp.dwStructSize = sizeof(urlComp);

	// Set required component lengths to non-zero so that they are cracked.
	urlComp.dwSchemeLength = (DWORD)-1;
	urlComp.dwHostNameLength = (DWORD)-1;
	urlComp.dwUrlPathLength = (DWORD)-1;
	urlComp.dwExtraInfoLength = (DWORD)-1;

	// Crack the URL.
	wstring wstrUrl = s2ws(strUrl);
	LPCWSTR pwszUrl = wstrUrl.c_str();

	if (!WinHttpCrackUrl(pwszUrl, (DWORD)wcslen(pwszUrl), 0, &urlComp))
	{
		printf("Error %u in WinHttpCrackUrl.\n", GetLastError());
	}
	else
	{
		UrlElements.wstrHostName.append(urlComp.lpszHostName, urlComp.dwHostNameLength);

		UrlElements.wstrUrlPath.append(urlComp.lpszUrlPath, urlComp.dwUrlPathLength);

		DWORD dwParamsSize = urlComp.dwExtraInfoLength > 1 ? urlComp.dwExtraInfoLength - 1 : 0;
		UrlElements.wstrParams.append(urlComp.lpszExtraInfo + 1, dwParamsSize);
	}

	return UrlElements;
}

VOID protocol::CWinHttp::init()
{
	m_uErrCode = 0;
	m_setHeaders.clear();
	m_strAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36";
	m_strAcceptType = "application/json, text/plain, */*";

	m_ReqData.lpReqData = nullptr;
	m_ReqData.dwReqDataSize = 0;
	m_RespData.lpHeaders = nullptr;
	m_RespData.dwHeaersSize = 0;
	m_RespData.lpData = nullptr;
	m_RespData.dwDataSize = 0;
}


VOID protocol::CWinHttp::reset()
{
	m_uErrCode = 0;
	m_ReqData.lpReqData = nullptr;
	m_ReqData.dwReqDataSize = 0;
	m_RespData.lpHeaders = nullptr;
	m_RespData.dwHeaersSize = 0;
	m_RespData.lpData = nullptr;
	m_RespData.dwDataSize = 0;
}

wstring protocol::CWinHttp::CreateHeaders()
{
	wstring wstrHeaders;
	if (m_setHeaders.empty())
	{
		return wstrHeaders;
	}

	vector<string> vecHeaders(m_setHeaders.begin(), m_setHeaders.end());
	for (size_t i = 0; i < vecHeaders.size() -1; ++i)
	{
		wstrHeaders += s2ws(vecHeaders[i]) + L"\r\n";
	}
	wstrHeaders += s2ws(vecHeaders[vecHeaders.size()-1]);

	//printf("%S\n", wstrHeaders.c_str());

	return wstrHeaders;
}




