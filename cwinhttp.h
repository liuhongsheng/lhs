#ifndef _CWINHTTP_H_
#define _CWINHTTP_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <Windows.h>
#include <winhttp.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>


#pragma comment(lib, "winhttp.lib")


using std::set;
using std::map;
using std::vector;
using std::string;
using std::wstring;


namespace protocol
{

// request method
#define POST ("POST")
#define GET ("GET")

// request/response header
#define DATE               "Date"
#define CONTENT_TYPE       "Content-Type"
#define TRANSFER_ENCODING  "Transfer-Encoding"
#define CONNECTION         "Connection"
#define SERVER             "Server"
#define EXPIRES            "Expires"
#define PRAGMA             "Pragma"
#define CACHE_CONTROL      "Cache-Control"
#define X_LANG             "X-LANG"
#define X_TIMEZONE         "X-Timezone"
#define X_TIMESTAMP        "X-Timestamp"
#define X_ARCH             "X-Arch"
#define X_ENTERPRISE       "X-Enterprise"
#define X_SUPPORT_I18N     "X-Support-i18n"
#define X_SUPPORT_WIFI     "X-Support-wifi"
#define SET_COOKIE         "Set-Cookie"




	typedef struct _URL_ELEMENT
	{
		wstring  wstrHostName;        // host name
		wstring  wstrUrlPath;         // URL-path
		wstring  wstrParams;          // params
	}URL_ELEMENT, *LPURL_ELEMENT;

	typedef struct _REQUEST
	{
		LPVOID lpReqData;
		DWORD dwReqDataSize;
	}
	REQUEST, *LPREQUEST;
	typedef const REQUEST *LPCREQUEST;

	typedef struct _RESPONSE
	{
		LPVOID lpHeaders;
		DWORD dwHeaersSize;

		LPVOID lpData;
		DWORD dwDataSize;
	}
	RESPONSE, *LPRESPONSE;
	typedef const RESPONSE *LPCRESPONSE;


	class CWinHttp
	{
	public:
		CWinHttp(){ init(); };
		CWinHttp(const string strUrl, const string strMethod = GET) : m_strUrl(strUrl), m_strMethod(strMethod){ init(); }
		~CWinHttp();

		int Request();
		int Request(const string strUrl);
		int Request(const string strUrl, const string strMethod);
		int Request(LPCWSTR pszAgentW, LPCWSTR pswzServerName, LPCWSTR pwszVerb, LPCWSTR pwszObjectName, 
			LPCWSTR *ppwszAcceptTypes, LPSTR pszData, DWORD dwReqDataSize, LPCWSTR lpszHeaders, DWORD dwHeadersLength, LPRESPONSE lpResponse);

		VOID SetUrl(const string strUrl){ m_strUrl = strUrl; }
		const string &GetUrl() const { return m_strUrl; }

		VOID SetReqData(PVOID lpReqData, DWORD dwReqDataSize);
		const REQUEST &GetReqData() const { return m_ReqData; }

		const RESPONSE &GetRespData() const { return m_RespData; }

		VOID SetMethod(const string strMethod){ m_strMethod = strMethod; }
		const string &GetMethod() const { return m_strMethod; }

		VOID SetAgent(const string strAgent) { m_strAgent = strAgent; }
		const string &GetAgent() const { return m_strAgent; }

		VOID SetReqHeaders(const string &strHeader){ m_setHeaders.insert(strHeader); }
		const set<string> &GetReqHeaders() const { return m_setHeaders; }

		uint32_t GetLastErrCode() const { return m_uErrCode; }
		const string &GetLastErrStr() const { return m_strErrStr; }

		static const URL_ELEMENT &ResolvUrl(string strUrl, URL_ELEMENT &UrlElements);

		VOID reset();

	private:
		VOID init();

		wstring CreateHeaders();

		VOID SetErrInfo(uint32_t uErrCode, const string strErrStr){ m_uErrCode = uErrCode; m_strErrStr = strErrStr; }

		CWinHttp(const CWinHttp&);
		const CWinHttp &operator=(const CWinHttp&);

	private:
		string m_strErrStr;
		uint32_t m_uErrCode;
		
		string m_strUrl;
		string m_strMethod;
		string m_strAgent;
		string m_strAcceptType;

		RESPONSE m_RespData;
		REQUEST m_ReqData;
		set<string> m_setHeaders;
	};
}

string ws2s(const wstring& ws);

wstring s2ws(const string& s);

#endif //_CWINHTTP_H_