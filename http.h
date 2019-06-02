#pragma once
#ifndef _HTTP_H_
#define _HTTP_H_
#include <Windows.h>
#include <winhttp.h>
#include <string>
#include <map>


#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "user32.lib")

using std::map;
using std::string;
using std::wstring;


typedef wstring WSTRING;

#define HTTP_CLOSE_HANDLE(handle) {if (handle) WinHttpCloseHandle(handle);}

#define STR_CHECK_SET_NULL(str) ((str.empty()) ? NULL : (str.c_str()))

#define INIT_NULL(v) { v = NULL; }

#define RET_OK 0
#define RET_FAIL (-1)

// request method
#define POST (L"POST")
#define GET (L"GET")

// general field
#define REQUEST_URL L"Request URL"
#define REQUEST_METHOD L"Request Method"
#define STATUS_CODE L"Status Code"
#define REMOTE_ADDRESS L"Remote Address"
#define REFERRER_POLICY L"Referrer Policy"

// request field
#define ACCEPT L"Accept"
#define ACCEPT_ENCODING L"Accept-Encoding"
#define ACCEPT_LANGUAGE L"Accept-Language"
#define CONNECTION L"Connection"
#define COOKIE L"Cookie"
#define HOST L"Host"
#define REFERER L"Referer"
#define USER_AGENT Ls"User-Agent"

// response field
#define ACCESS_CONTROL_ALLOW_ORIGIN L"Access-Control-Allow-Origin"
#define CACHE_CONTROL L"Cache-Control"
#define CONNECTION L"Connection"
#define CONTENT_ENCODING L"Content-Encoding"
#define CONTENT_LENGTH L"Content-Length"
#define CONTENT_TYPE L"Content-Type"
#define DATE L"Date"
#define EXPIRES L"Expires"
#define REQUEST_CONTEXT L"Request-Context"
#define VARY L"Vary"
#define X_CACHE_HIT L"x-cache-hit"
#define X_CONTENT_TYPE_OPTIONS L"X-Content-Type-Options"



namespace protocol{

	typedef struct _URL_ELEMENT
	{
		WSTRING  wstrHostName;        // host name
		WSTRING  wstrUrlPath;         // URL-path
		WSTRING  wstrParams;          // params
	}URL_ELEMENT, *LPURL_ELEMENT;

	typedef struct _REQUEST_ATTR
	{
		WSTRING          wstrAgentW;
		DWORD            dwAccessType;
		WSTRING          wstrProxyW;
		WSTRING          wstrProxyBypassW;
		DWORD            dwFlagsOpen;

		WSTRING          wstrServerName;
		INTERNET_PORT    nServerPort;
		DWORD            dwReserved;

		WSTRING          wstrMethod;
		WSTRING          wstrUri;
		WSTRING          wstrVersion;
		WSTRING          wstrReferrer;
		WSTRING          wstrAcceptTypes;
		DWORD            dwFlagsReq;
	}
	REQUEST_ATTR, *LPREQUEST_ATTR;

	class CRequest
	{
	public:
		enum REQUSET_TYPE_E
		{
			ENUM_START = 0,
			HTTP_REQUEST = 1,
			HTTPS_REQUEST = 2,
			ENUM_END
		};

		CRequest() = default;
		CRequest(const WSTRING wstrUrl, const WSTRING wstrRequestData = L"", REQUSET_TYPE_E eRequestType = HTTP_REQUEST) : 
			m_wstrUrl(wstrUrl), m_wstrRequestData(wstrRequestData), m_eRequestType(eRequestType)
		{
			InitAttr();
		}

		~CRequest();

		CRequest & CreateRequest(VOID);

		VOID CloseHandle(VOID);
		
		VOID SetUrl(const WSTRING wstrUrl) { m_wstrUrl = wstrUrl; }
		const WSTRING GetUrl() const { return m_wstrUrl; }

		VOID SetRequestType(const REQUSET_TYPE_E eRequestType) { m_eRequestType = eRequestType; }
		const  REQUSET_TYPE_E GetRequestType() const { return m_eRequestType; }

		VOID SetPort(const INTERNET_PORT nServerPort) { m_Attr.nServerPort = nServerPort; }
		const INTERNET_PORT GetPort() const { return m_Attr.nServerPort; }

		const WSTRING GetMethod() const { return m_Attr.wstrMethod; }

		VOID SetRequestData(const WSTRING wstrRequestData) { m_wstrRequestData = wstrRequestData; }
		const WSTRING GetRequestData() const { return m_wstrRequestData; }

		VOID SetSSL(HCERTSTORE hCertFile);

		HINTERNET GetSessionHandle() const { return m_hSession; }

		HINTERNET GetConnectHandle() const { return m_hConnect; }

		HINTERNET GetRequestHandle() const { return m_hRequest; }
		
		static const URL_ELEMENT &ResolvUrl(LPCWSTR pwszUrl, URL_ELEMENT &UrlElements);

		BOOL AddRequestHeaders(map<wstring, wstring> mapHeaders, DWORD dwModifiers = WINHTTP_ADDREQ_FLAG_ADD);
	
	private:

		VOID InitAttr(VOID);

		CRequest(const CRequest &);
		const CRequest &operator=(const CRequest&);
	
	private:
		WSTRING m_wstrUrl;
		REQUEST_ATTR m_Attr;
		HINTERNET m_hSession;
		HINTERNET m_hConnect;
		HINTERNET m_hRequest;
		WSTRING m_wstrRequestData;
		URL_ELEMENT m_wstrUrlElements;
		REQUSET_TYPE_E m_eRequestType;
	};


	typedef struct _RESPONSE
	{
		LPVOID lpRespData;
		DWORD dwDataSize;
	}RESPONSE, *LPRESPONSE;


	class CHttp
	{
	public:
		CHttp() { m_Response.dwDataSize = 0; m_Response.lpRespData = nullptr; };
		~CHttp(){ delete[] m_Response.lpRespData; };

	public:
		INT SendRequest(const CRequest &oCRequest);
		
		PVOID GetResponseData() const { return m_Response.lpRespData; }

		DWORD GetResponseDataSize() const { return m_Response.dwDataSize; }

	private:

		BOOL ReadResponseData(HINTERNET hRequest);

		CHttp(const CHttp &);
		const CHttp & operator=(const CHttp &);

	private:
		RESPONSE m_Response;
	};
}

std::wstring CharToWchar(const char* c, size_t m_encode = CP_ACP);

std::string WcharToChar(const wchar_t* wp, size_t m_encode = CP_ACP);

#endif