#include "httpcurl.h"
#include <sstream>
#include <vector>


using std::vector;
using std::string;
using std::stringstream;

static vector<string> Split2String(const string& strInput, const string& strSeparator, bool returnIncludeEmptyStr = false)
{
	vector<string> vecResult;

	size_t last = 0;
	size_t index = strInput.find_first_of(strSeparator, last);
	while (string::npos != index)
	{
		string strElement = strInput.substr(last, index - last);
		if ((!strElement.empty()) || returnIncludeEmptyStr)
		{
			vecResult.push_back(strElement);
		}
		last = index + 1;
		index = strInput.find_first_of(strSeparator, last);
	}

	string strElement = strInput.substr(last, index - last);
	if ((!strElement.empty()) || returnIncludeEmptyStr)
	{
		vecResult.push_back(strElement);
	}

	return vecResult;
}



protocol::CHttpcurl::CHttpcurl() :m_pCurl(NULL)
{
	Init();
}

protocol::CHttpcurl::~CHttpcurl()
{
	if (m_pCurl)
	{
		curl_easy_cleanup(m_pCurl);
		m_pCurl = NULL;
	}
}

int protocol::CHttpcurl::Init(void)
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

	m_strCookieFile = "";

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
	m_setHeaders.insert("Content-Encoding: UTF-8");

	// output debug info (for debug only)
	// curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, true);
	curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, false);

	return 0;
}

void protocol::CHttpcurl::AddHttpHeader(const string& strHeader)
{
	m_setHeaders.insert(strHeader);
}

int protocol::CHttpcurl::SetUserPwd(const std::string& strUserPwd)
{
	if (NULL == m_pCurl)
		return -1;

	curl_easy_setopt(m_pCurl, CURLOPT_USERPWD, strUserPwd.c_str());
	return 0;
}

int protocol::CHttpcurl::Post(const string& strUrl, const string& strRequest, string& strReply)
{
	return Request(strUrl, HTTP_POST, strRequest, strReply);
}

int protocol::CHttpcurl::Get(const string& strUrl, string& strReply)
{
	return Request(strUrl, HTTP_GET, "", strReply);
}


int protocol::CHttpcurl::SetTimeOut(int iTimeOut)
{
	if (m_pCurl == NULL)
		return -1;

	curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, iTimeOut);

	return 0;
}


int protocol::CHttpcurl::SetCert(void)
{
	if (m_pCurl == NULL)
		return -1;

	if (NULL == m_lpSSLAttr)
	{
		return -2;
	}

	string strCertDir = m_lpSSLAttr->strCertDir;
	string strSSLKey = strCertDir + m_lpSSLAttr->strSSLKey;
	string strSSLCert = strCertDir + m_lpSSLAttr->strSSLCert;
	string strSSLPwd = m_lpSSLAttr->strSSLPwd;
	string strSSLType = m_lpSSLAttr->strSSLType;

	curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 1L);
	curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 1L);
	curl_easy_setopt(m_pCurl, CURLOPT_SSLKEY, strSSLKey.c_str());
	curl_easy_setopt(m_pCurl, CURLOPT_SSLCERT, strSSLCert.c_str());
	curl_easy_setopt(m_pCurl, CURLOPT_SSLCERTPASSWD, strSSLPwd.c_str());
	curl_easy_setopt(m_pCurl, CURLOPT_SSLCERTTYPE, strSSLType.c_str());

	return 0;
}


size_t protocol::CHttpcurl::WriteDataCallback(void *ptr, size_t size, size_t nCount, void *pData)
{
	stringstream* pTmp = (stringstream*)pData;
	pTmp->write((char*)ptr, size * nCount);
	return size * nCount;
}

int protocol::CHttpcurl::Request(const string& strUrl, const E_HTTP_ACTION eAct, const string& strRequest, string& strReply)
{
	int iRet = CURLE_OK;
	stringstream ssReply;

	do 
	{
		if (m_pCurl == NULL)
		{
			iRet = -1;
			break;
		}

		if (strUrl.empty())
		{
			iRet = -2;
			break;
		}

		if (eAct <= protocol::CHttpcurl::ENUM_START || eAct >= protocol::CHttpcurl::ENUM_END)
		{
			iRet = -3;
			break;
		}

		if (protocol::CHttpcurl::HTTP_POST == eAct && strRequest.empty())
		{
			iRet = -4;
			break;;
		}

		SetHttpAction(eAct);
		curl_easy_setopt(m_pCurl, CURLOPT_URL, strUrl.c_str());
		curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &ssReply);
		curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, WriteDataCallback);
		curl_easy_setopt(m_pCurl, CURLOPT_COOKIEFILE, "");

		if (protocol::CHttpcurl::HTTP_POST == eAct)
		{
			curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, strRequest.c_str());
			curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDSIZE, strRequest.length());
		}

		struct curl_slist *cookies = NULL;
		struct curl_slist *headers = NULL;
		SetHeaders(m_setHeaders, headers);
		iRet = curl_easy_perform(m_pCurl);

		if (CURLE_OK == iRet)
		{
			strReply = ssReply.str();
			SetRemoteIP();
			SetCookie(cookies);
		}
		
		if (NULL != headers) curl_slist_free_all(headers);
		if(NULL != cookies) curl_slist_free_all(cookies);

	} while (0);

	return iRet;
}

void protocol::CHttpcurl::SetHeaders(const set<string> &setHeaders, struct curl_slist *headers)
{
	if (!setHeaders.empty())
	{
		for (set<string>::iterator it = setHeaders.begin(); it != setHeaders.end(); ++it)
			headers = curl_slist_append(headers, it->c_str());
	}

	if(CURLE_OK != curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, headers))
	{
		printf("Error %d in curl_easy_setopt[SetHttpAction]\n");
	}
}

void protocol::CHttpcurl::SetRemoteIP(void)
{
	char *ip;
	CURLcode cRet;
	cRet = curl_easy_getinfo(m_pCurl, CURLINFO_PRIMARY_IP, &ip);
	if ((CURLE_OK == cRet) && ip)
	{
		m_strRemoteIP = ip;
	}
}

void protocol::CHttpcurl::SetHttpAction(const E_HTTP_ACTION eAct)
{
	CURLcode cRet = CURLE_OK;
	CURLoption option;
	switch (eAct)
	{

	case HTTP_GET:
		option = CURLOPT_HTTPGET;
		break;

	case HTTP_POST:
		option = CURLOPT_POST;
		break;

	case HTTP_PUT:
		option = CURLOPT_PUT;
		break;

	default:
		break;
	}

	if (CURLE_OK != curl_easy_setopt(m_pCurl, option, 1L))
	{
		printf("Error %d in curl_easy_setopt[SetHttpAction]\n");
	}
}

void protocol::CHttpcurl::SetCookie(struct curl_slist *cookies)
{
	/* extract all known cookies */
	CURLcode res = CURLE_OK;
	stringstream ssCookies;

	res = curl_easy_getinfo(m_pCurl, CURLINFO_COOKIELIST, &cookies);
	if (!res && cookies)
	{
		/* a linked list of cookies in cookie file format */
		struct curl_slist *each = cookies;
		while (each) 
		{
			ssCookies << each->data;
			each = each->next;
		}
	}

	string strCookie = ssCookies.str();
	//format : hostname\tFALSE\tpath\tFALSE\tExpiry\tname\tvalue
	vector<string> vecCookies = Split2String(strCookie, "\t");
	for (size_t index = 5; index < vecCookies.size(); index += 2) // The first five fields are fixed
	{
		m_mapCookies[vecCookies[index]] = vecCookies[index + 1];
	}
}
