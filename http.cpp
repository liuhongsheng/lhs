#include "http.h"
#include <stdio.h>
#include <vector>
#include <list>


protocol::CRequest::~CRequest()
{
	CloseHandle();
}

protocol::CRequest & protocol::CRequest::CreateRequest(VOID)
{
	LPCWSTR pszAgentW = STR_CHECK_SET_NULL(m_Attr.wstrAgentW);
	LPCWSTR pszProxyW = STR_CHECK_SET_NULL(m_Attr.wstrProxyW);
	LPCWSTR pszProxyBypassW = STR_CHECK_SET_NULL(m_Attr.wstrProxyBypassW);
	LPCWSTR pswzServerName = STR_CHECK_SET_NULL(m_Attr.wstrServerName);
	LPCWSTR pwszVerb = STR_CHECK_SET_NULL(m_Attr.wstrMethod);
	LPCWSTR pwszObjectName = STR_CHECK_SET_NULL(m_Attr.wstrUri);
	LPCWSTR pwszVersion = STR_CHECK_SET_NULL(m_Attr.wstrVersion);
	LPCWSTR pwszReferrer = STR_CHECK_SET_NULL(m_Attr.wstrReferrer);

	LPCWSTR pwszAcceptTypes = m_Attr.wstrAcceptTypes.empty() ? NULL : (m_Attr.wstrAcceptTypes.c_str());
	LPCWSTR *ppwszAcceptTypes = NULL == pwszAcceptTypes ? NULL : &pwszAcceptTypes;

	// Use WinHttpOpen to obtain a session handle.
	m_hSession = WinHttpOpen(pszAgentW,
		m_Attr.dwAccessType,
		pszProxyW,
		pszProxyBypassW,
		m_Attr.dwFlagsOpen);

	// Specify an HTTP server.
	if (m_hSession)
	{
		m_hConnect = WinHttpConnect(m_hSession, pswzServerName, m_Attr.nServerPort, 0);
	}
	else
	{
		printf("Error %u in WinHttpOpen.\n", GetLastError());
	}

	// Create an HTTP request handle.
	if (m_hConnect)
	{
		m_hRequest = WinHttpOpenRequest(m_hConnect,
			pwszVerb,
			pwszObjectName,
			pwszVersion,
			pwszReferrer,
			ppwszAcceptTypes,
			m_Attr.dwFlagsReq);
	}
	else
	{
		printf("Error %u in WinHttpConnect.\n", GetLastError());
	}

	if (!m_hRequest)
	{
		printf("Error %u in WinHttpOpenRequest.\n", GetLastError());
	}

	return *this;
}

VOID protocol::CRequest::CloseHandle(VOID)
{
	HTTP_CLOSE_HANDLE(m_hSession)
	HTTP_CLOSE_HANDLE(m_hConnect)
	HTTP_CLOSE_HANDLE(m_hRequest)
}

const protocol::URL_ELEMENT &protocol::CRequest::ResolvUrl(const LPCWSTR pwszUrl, URL_ELEMENT &UrlElements)
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

BOOL protocol::CRequest::AddRequestHeaders(map<wstring, wstring> mapHeaders, DWORD dwModifiers /* = WINHTTP_ADDREQ_FLAG_ADD*/)
{
	for (auto &v : mapHeaders)
	{
		if (!WinHttpAddRequestHeaders(m_hRequest, (v.first + L": " + v.second).c_str(), (ULONG)-1L, dwModifiers))
			return FALSE;
	}

	return TRUE;
}

VOID protocol::CRequest::InitAttr(VOID)
{
	ResolvUrl(m_wstrUrl.c_str(), m_wstrUrlElements);

	m_Attr.wstrAgentW = L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36";
	m_Attr.dwAccessType = WINHTTP_ACCESS_TYPE_DEFAULT_PROXY;
	m_Attr.dwFlagsOpen = 0;
		
	m_Attr.wstrServerName = m_wstrUrlElements.wstrHostName;
	m_Attr.nServerPort = INTERNET_DEFAULT_HTTP_PORT;
	m_Attr.dwReserved = 0;
	
	m_Attr.wstrMethod = L"GET";
	m_Attr.dwFlagsReq = 0;
}


INT protocol::CHttp::SendRequest(const CRequest &oCRequest)
{

	BOOL bResults = FALSE;

	// Send a request.

	HINTERNET hRequest = oCRequest.GetRequestHandle();
	if (hRequest)
	{
		if (POST == oCRequest.GetMethod().c_str())
		{
			bResults = WinHttpSendRequest(hRequest,
				WINHTTP_NO_ADDITIONAL_HEADERS, 0,
				WINHTTP_NO_REQUEST_DATA, 0,
				oCRequest.GetRequestData().size(), 0);

			DWORD dwBytesWritten = 0;
			size_t dwBytes = (oCRequest.GetRequestData().size() + 1) * sizeof(WCHAR);
			LPSTR pszData = new CHAR[dwBytes];
			memcpy(pszData, oCRequest.GetRequestData().c_str(), dwBytes);

			bResults = WinHttpWriteData(hRequest, pszData,
				(DWORD)strlen(pszData),
				&dwBytesWritten);
		}
		else
		{
			bResults = WinHttpSendRequest(hRequest,
				WINHTTP_NO_ADDITIONAL_HEADERS, 0,
				WINHTTP_NO_REQUEST_DATA, 0,
				0, 0);
		}
	}

	// End the request.
	if (bResults)
	{
		bResults = WinHttpReceiveResponse(oCRequest.GetRequestHandle(), NULL);
	}
	else
	{
		printf("Error %u in WinHttpSendRequest.\n", GetLastError());
		return RET_FAIL;
	}

	// Keep checking for data until there is nothing left.
	if (bResults)
	{
		bResults = ReadResponseData(oCRequest.GetRequestHandle());
	}
	else
	{
		printf("Error %u in WinHttpReceiveResponse.\n", GetLastError());
		return RET_FAIL;
	}

	// Report any errors.
	if (!bResults)
	{
		printf("Error %u in ReadResponseData.\n", GetLastError());
		return RET_FAIL;
	}

	return 0;
}


BOOL protocol::CHttp::ReadResponseData(HINTERNET hRequest)
{
	DWORD dwSize = 0;
	std::vector<RESPONSE> vecRespData;
	
	if (m_Response.lpRespData != nullptr) delete[] m_Response.lpRespData;

	BOOL bResults = TRUE;
	do
	{
		RESPONSE response;
		response.dwDataSize = 0;
		response.lpRespData = nullptr;

		// Check for available data.
		if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
		{
			printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());
			bResults = FALSE;
			break;
		}

		// Allocate space for the buffer.
		response.lpRespData = new char[dwSize + 1];
		if (nullptr == response.lpRespData)
		{
			printf("Out of memory\n");
			dwSize = 0;
			bResults = FALSE;
			break;
		}
		else
		{
			// Read the data.
			ZeroMemory(response.lpRespData, dwSize + 1);

			if (!WinHttpReadData(hRequest, response.lpRespData, dwSize, &response.dwDataSize))
			{
				printf("Error %u in WinHttpReadData.\n", GetLastError());
				bResults = FALSE;
				break;
			}

			vecRespData.push_back(response);
			m_Response.dwDataSize += response.dwDataSize;
		}
	} while (dwSize > 0);

	if (!bResults)
	{
		for (size_t i = 0; i < vecRespData.size(); ++i)
		{
			if (nullptr != vecRespData[i].lpRespData) delete[] vecRespData[i].lpRespData;
		}
	}
	else
	{
		size_t iInterval = 0;
		m_Response.lpRespData = new char[m_Response.dwDataSize + 8096];
		ZeroMemory(m_Response.lpRespData, m_Response.dwDataSize + 8096);
		for (size_t i = 0; i < vecRespData.size() - 1; ++i)
		{
			RESPONSE resp = vecRespData[i];
			memcpy((char*)m_Response.lpRespData + iInterval, (char*)resp.lpRespData, resp.dwDataSize);
			iInterval += resp.dwDataSize;
			delete[] resp.lpRespData;
		}	
	}

	return bResults;
}

std::wstring CharToWchar(const char* c, size_t m_encode /*= CP_ACP*/)
{
	std::wstring str;
	int len = MultiByteToWideChar(m_encode, 0, c, strlen(c), NULL, 0);
	wchar_t*    m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(m_encode, 0, c, strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	str = m_wchar;
	delete m_wchar;
	return str;
}

std::string WcharToChar(const wchar_t* wp, size_t m_encode /*= CP_ACP*/)
{
	std::string str;
	int len = WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
	char    *m_char = new char[len + 1];
	WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	str = m_char;
	delete m_char;
	return str;
}