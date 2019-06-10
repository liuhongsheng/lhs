#ifndef _HTTPCURL_H_
#define _HTTPCURL_H_

#include "curl.h"

#include <set>
#include <map>
#include <string>


namespace protocol
{
	using std::set;
	using std::map;
	using std::string;

	class CHttpcurl
	{
	public:
		CHttpcurl();
		~CHttpcurl();

		typedef enum
		{
			ENUM_START = 0,
			HTTP_GET = 1,
			HTTP_POST = 2,
			HTTP_PUT = 3,
			HTTP_DELETE = 4,
			ENUM_END
		}E_HTTP_ACTION;

		typedef struct _SSL_ATTR
		{
			string strCertDir;
			string strSSLKey;
			string strSSLCert;
			string strSSLPwd;
			string strSSLType;
		}SSL_ATTR, *LPSSL_ATTR;

		int SetCert();

		int SetTimeOut(int iTimeOut);

		int SetUserPwd(const string& strUserPwd);

		void AddHttpHeader(const string& strHeader);

		void SetCookieFile(const string &strCookieFile) { m_strCookieFile = strCookieFile; }

		void SetSSLAttr(const LPSSL_ATTR lpSSLAttr) { m_lpSSLAttr = lpSSLAttr; }

		int Get(const string& strUrl, string& strReply);

		int Post(const string& strUrl, const string& strRequest, string& strReply);

		string GetRemoteIP(void) const { return m_strRemoteIP; }

		map<string, string> GetCookies() const { return m_mapCookies; }

	private:
		int Init(void);

		void SetRemoteIP(void);

		void SetCookie(struct curl_slist *cookies);

		void SetHttpAction(const E_HTTP_ACTION eAct);

		void SetHeaders(const set<string> &setHeaders, struct curl_slist *headers);

		int Request(const string& strUrl, const E_HTTP_ACTION eAct, const string& strRequest, string& strReply);

		static size_t WriteDataCallback(void *ptr, size_t size, size_t nCount, void *pData);

		CURL *m_pCurl;
		LPSSL_ATTR m_lpSSLAttr;
		string m_strRemoteIP;
		string m_strCookieFile;
		set<string> m_setHeaders;
		map<string, string> m_mapCookies;
	};

}

#endif //_HTTPCURL_H_