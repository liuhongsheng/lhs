
#ifndef _HTTPCURL_H_
#define _HTTPCURL_H_

#include <string>
#include <set>
#include <curl/curl.h>

namespace tools
{
    using std::set;
    using std::string;

    class CHttpcurl
	{
	public:
        CHttpcurl();
        ~CHttpcurl();

		int SetCert();
        int SetCertForParam(const string& strCacertDir, const string& strPwd);

        int SetTimeOut(int iTimeOut);
        int SetUserPwd( const string& strUserPwd );
        void AddHttpHeader(const string& strHeader);

        void setCookieFile(const string &strFile) {m_strCookieFile = strFile;};

        int Get(const string& strUrl, string& strReply);
        int Post(const string& strUrl, const string& strRequest, string& strReply);

        string GetRemoteIP(){ return m_strRemoteIP;}

	private:
		int Init();
		static size_t WriteDataCallback(void *ptr, size_t size, size_t nCount, void *pData);

		CURL *m_pCurl;
        string m_strRemoteIP;
        string m_strCookieFile;

        set<string> m_setHeaders;
	};
}

#endif //_HTTPCURL_H_

