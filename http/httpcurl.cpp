#include "httpcurl.h"
#include <sstream>

using namespace std;
using namespace tools;

CHttpcurl::CHttpcurl() :m_pCurl(NULL)
{
   Init();
}

CHttpcurl::~CHttpcurl()
{
    if (m_pCurl)
    {
        curl_easy_cleanup(m_pCurl);
        m_pCurl = NULL;
    }
}

int CHttpcurl::Init()
{
    if (m_pCurl)
    {
        curl_easy_cleanup(m_pCurl);
        m_pCurl = NULL;
    }

    m_pCurl = curl_easy_init();
    if (m_pCurl == NULL)
    {
        return -1;
    }

    // set connection timeout to 10's
    curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, 10);

    // set timeout to 30's
    curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, 30);

    // accept identiy, deflate, and gzip encoding. (Accept-Encoding: )
    curl_easy_setopt(m_pCurl, CURLOPT_ENCODING, "gzip, deflate");

    // set user-agent to that of MSIE6
    curl_easy_setopt(m_pCurl, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)");

    // let curl to follow location (auto handle HTTP 301, 302)
    curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1);

    m_setHeaders.clear();
    m_setHeaders.insert("Accept-Language: zh-cn");

    // output debug info (for debug only)
    // curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, true);
    curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, false);

    return 0;
}

void CHttpcurl::AddHttpHeader(const string& strHeader)
{
    m_setHeaders.insert(strHeader);
}

int CHttpcurl::SetUserPwd( const std::string& strUserPwd )
{
    if( NULL == m_pCurl )
        return -1;

    curl_easy_setopt( m_pCurl, CURLOPT_USERPWD, strUserPwd.c_str() );
    return 0;
}

int CHttpcurl::Post(const string& strUrl, const string& strRequest, string& strReply)
{
    if (m_pCurl == NULL)
        return -1;

    int iRet;
    if (strUrl.empty())
    {
        return -2;
    }

    stringstream ssReply;

    curl_easy_setopt(m_pCurl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &ssReply);
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, WriteDataCallback);

    // set post data
    if (strRequest.empty())
    {
        return -3;
    }
    curl_easy_setopt(m_pCurl, CURLOPT_POST, true);
    curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, strRequest.c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDSIZE, strRequest.length());

    curl_easy_setopt(m_pCurl, CURLOPT_COOKIEFILE, "/dev/null");
    struct curl_slist *headers = NULL;
    if (!m_setHeaders.empty())
    {
        for (set<string>::iterator it = m_setHeaders.begin(); it != m_setHeaders.end(); ++it)
            headers = curl_slist_append(headers, it->c_str());
    }
    curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, headers);

    iRet = curl_easy_perform(m_pCurl);
    if (iRet == CURLE_OK)
    {
        strReply = ssReply.str();

        CURLcode return_code;
        char *ip = nullptr;
        return_code = curl_easy_getinfo(m_pCurl, CURLINFO_PRIMARY_IP, &ip);
        if((CURLE_OK==return_code) && ip)
        {
            m_strRemoteIP = ip;
        }
    }

    if (headers)
        curl_slist_free_all(headers);

    return (iRet == CURLE_OK) ? 0 : iRet;
}

int CHttpcurl::Get(const string& strUrl, string& strReply)
{
    if (m_pCurl == NULL)
        return -1;

    int iRet;
    if (strUrl.empty())
    {
        return -2;
    }

    curl_easy_setopt(m_pCurl, CURLOPT_URL, strUrl.c_str());
    stringstream ssReply;
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &ssReply);
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, WriteDataCallback);

    curl_easy_setopt(m_pCurl, CURLOPT_HTTPGET, true);
    curl_easy_setopt(m_pCurl, CURLOPT_COOKIEFILE, m_strCookieFile.c_str());

    struct curl_slist *headers = NULL;
    if (!m_setHeaders.empty())
    {
        for (set<string>::iterator it = m_setHeaders.begin(); it != m_setHeaders.end(); ++it)
            headers = curl_slist_append(headers, it->c_str());
    }
    curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);

    iRet = curl_easy_perform(m_pCurl);
    if (iRet == CURLE_OK)
    {
        strReply = ssReply.str();
    }

    if (headers)
        curl_slist_free_all(headers);

    return (iRet == CURLE_OK) ? 0 : iRet;
}


int CHttpcurl::SetTimeOut(int iTimeOut)
{
    if (m_pCurl == NULL)
        return -1;

    curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, iTimeOut);

    return 0;
}


int CHttpcurl::SetCert()
{
    if (m_pCurl == NULL)
        return -1;

    string strCacertDir = "/huishoubao/cert/";
    curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 1L);
    curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(m_pCurl, CURLOPT_SSLKEY, (strCacertDir + "apiclient_key.pem").c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_SSLCERT, (strCacertDir + "apiclient_cert.pem").c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_SSLCERTPASSWD, "1229290102");
    curl_easy_setopt(m_pCurl, CURLOPT_SSLCERTTYPE, "PEM");

    return 0;
}


int CHttpcurl::SetCertForParam(const std::string& strCacertDir, const std::string& strPwd)
{
    if (m_pCurl == NULL)
        return -1;

    if (strCacertDir.empty() || strPwd.empty())
        return -1;

    curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 1L);
    curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(m_pCurl, CURLOPT_SSLKEY, (strCacertDir + "apiclient_key.pem").c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_SSLCERT, (strCacertDir + "apiclient_cert.pem").c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_SSLCERTPASSWD, strPwd.c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_SSLCERTTYPE, "PEM");

    return 0;
}




size_t CHttpcurl::WriteDataCallback(void *ptr, size_t size, size_t nCount, void *pData)
{
    stringstream* pTmp = (stringstream*)pData;
    pTmp->write((char*)ptr, size * nCount);
    return size * nCount;
}
