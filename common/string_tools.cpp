#include "common/string_tools.h"
#include <ctime>
#include <ctype.h>
#include <iomanip>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>


using namespace std;


void tools::CStringTools::Show(const vector<unsigned int>& vecInput)
{
    size_t maxSize = vecInput.size();
    for (size_t index = 0; index < maxSize; index++)
    {
		cout << tools::CStringTools::Int2String(vecInput[index]) << endl;
    }
}

void tools::CStringTools::Show(const map<string, string>& mapInput)
{
    map<string, string>::const_iterator iter = mapInput.begin();
    const map<string, string>::const_iterator iterEnd = mapInput.end();
    for (; iter != iterEnd; ++iter)
    {
		cout << iter->first + " : " + iter->second << endl;
    }
}


string tools::CStringTools::Float2String(double fInput, int nplaces /* = 3 */)
{
	stringstream ssInput;
	ssInput << std::setprecision(nplaces) << fInput;
	return ssInput.str();
}

string tools::CStringTools::Int2String(int iInput)
{
    stringstream ssInput;
    ssInput << iInput;
    return ssInput.str();
}

string tools::CStringTools::Int2String(unsigned int iInput)
{
    stringstream ssInput;
    ssInput << iInput;
    return ssInput.str();
}

string tools::CStringTools::Int2String(unsigned long iInput)
{
    stringstream ssInput;
    ssInput << iInput;
    return ssInput.str();
}


vector<string> tools::CStringTools::Split2String(const string& strInput, const string& strSeparator, bool returnIncludeEmptyStr/*false*/)
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

vector<unsigned int> tools::CStringTools::Split2Uint(const string& strInput, const string& strSeparator)
{
    vector<unsigned int> vecResult;

    size_t last = 0;
    size_t index = strInput.find_first_of(strSeparator, last);
    while (string::npos != index)
    {
        string strElement = strInput.substr(last, index - last);
        if (!strElement.empty())
        {
            vecResult.push_back(::atoi(strElement.c_str()));
        }
        last = index + 1;
        index = strInput.find_first_of(strSeparator, last);
    }
    string strElement = strInput.substr(last, index - last);
    if (!strElement.empty())
    {
        vecResult.push_back(::atoi(strElement.c_str()));
    }

    return vecResult;
}

vector<int> tools::CStringTools::Split2Int(const string& strInput, const string& strSeparator)
{
    vector<int> vecResult;

    size_t last = 0;
    size_t index = strInput.find_first_of(strSeparator, last);
    while (string::npos != index)
    {
        string strElement = strInput.substr(last, index - last);
        if (!strElement.empty())
        {
            vecResult.push_back(::atoi(strElement.c_str()));
        }
        last = index + 1;
        index = strInput.find_first_of(strSeparator, last);
    }
    string strElement = strInput.substr(last, index - last);
    if (!strElement.empty())
    {
        vecResult.push_back(::atoi(strElement.c_str()));
    }

    return vecResult;
}

map<string, string> tools::CStringTools::SplitTwice2String(const string& strInput, const string& strSeparator, const string& strSeparator2)
{
    map<string, string> mapResult;

    vector<string> vec = tools::CStringTools::Split2String(strInput, strSeparator);
    size_t maxVec = vec.size();
    for (size_t iIndex = 0; iIndex < maxVec; iIndex++)
    {
        vector<string> vecTmp = tools::CStringTools::Split2String(vec[iIndex], strSeparator2);
        if (2 == vecTmp.size())
        {
            mapResult.insert(pair<string, string>(vecTmp[0], vecTmp[1]));
        }
        else if (1 == vecTmp.size())
        {
            mapResult.insert(pair<string, string>(vecTmp[0], ""));
        }
        else
        {
            continue;
        }
    }
    return mapResult;
}

map<unsigned int, unsigned int> tools::CStringTools::SplitTwice2Uint(const string& strInput, const string& strSeparator, const string& strSeparator2)
{
    map<unsigned int, unsigned int> mapResult;

    vector<string> vec = tools::CStringTools::Split2String(strInput, strSeparator);
    size_t maxVec = vec.size();
    for (size_t iIndex = 0; iIndex < maxVec; iIndex++)
    {
        vector<string> vecTmp = tools::CStringTools::Split2String(vec[iIndex], strSeparator2);
        if (2 == vecTmp.size())
        {
            mapResult.insert(pair<unsigned int, unsigned int>(::atoi(vecTmp[0].c_str()), ::atoi(vecTmp[1].c_str())));
        }
        else if (1 == vecTmp.size())
        {
            mapResult.insert(pair<unsigned int, unsigned int>(::atoi(vecTmp[0].c_str()), 0));
        }
        else
        {
            continue;
        }
    }
    return mapResult;
}

string tools::CStringTools::Trim(const string& strInput)
{
    string strTemp = strInput;
    size_t begin = 0;
    begin = strTemp.find(" ", begin);//查找空格在str中第一次出现的位置
    while(string::npos != begin)//表示字符串中存在空格
    {
        strTemp.replace(begin, 1, "");//用空串替换str中从begin开始的1个字符
        begin = strTemp.find(" ",begin);//查找空格在替换后的str中第一次出现的位置
    }
    return strTemp;
}
string tools::CStringTools::TrimTab(const string& strInput)
{
    string strTemp = strInput;
    size_t begin = 0;
    begin = strTemp.find("\t", begin);//查找Tab在str中第一次出现的位置
    while(string::npos != begin)//表示字符串中存在空格
    {
        strTemp.replace(begin, 1, "");//用空串替换str中从begin开始的1个字符
        begin = strTemp.find("\t",begin);//查找Tab在替换后的str中第一次出现的位置
    }
    return strTemp;
}


//去除字符串头部和尾部的空格
string tools::CStringTools::TrimLR(const string & strInput)
{
    if (strInput.empty())
        return strInput;
    return strInput.substr(strInput.find_first_not_of(" "), strInput.find_last_not_of(" ") + 1);
}


string tools::CStringTools::TrimLRReal(const string &strInput)
{
    int i = 0;
    int j = strInput.length() - 1;

    while (i <= j && ::isspace(strInput[i]))
    {
        i++;
    }
    while (j >=0 && ::isspace(strInput[j]))
    {
        j--;
    }

    if (i >= j)
    {
        return "";
    }

    return strInput.substr(i, j - i + 1);
}

static inline char fromHex(const char &ch)
{
    return isdigit(ch) ? ch-'0' : ch-'A'+10;
}

std::string tools::CStringTools::UrlDecode(const std::string& strUrl)
{
    string strResult;
    for(size_t index = 0; index < strUrl.size(); index++)
    {
        char ch = 0;
        if(strUrl[index] == '%')
        {
            ch = (fromHex(strUrl[index+1])<<4);
            ch |= fromHex(strUrl[index+2]);
            index += 2;
        }
        else if(strUrl[index] == '+')
        {
            ch = ' ';
        }
        else
        {
            ch = strUrl[index];
        }
        strResult += (char)ch;
    }
    return strResult;
}

static inline char toHex(const char &ch)
{
    return ch > 9 ? ch -10 + 'A': ch + '0';
}

//URL 编码
std::string tools::CStringTools::UrlEncode(std::string strUrl)
{
    std::string strResult;
    for(size_t index = 0; index < strUrl.size(); index++)
    {
        char buf[5];
        memset( buf, 0, 5 );
        if (isalnum((char)strUrl[index]))
        {
           buf[0] = strUrl[index];
        }
        else
        {
           buf[0] = '%';
           buf[1] = '%';
           buf[2] = toHex( (char)strUrl[index] >> 4 );
           buf[3] = toHex( (char)strUrl[index] % 16);
        }
        strResult += (char *)buf;
    }
    return strResult;
}

//----------------------------------------------------
//除了  0~9 a~z A~Z . - _  之外的符号都进行编码
//char*out 需要准备3倍待转码字符串的长度
static void urlencode(char* in, char* out){
    //unsigned int len_in=strlen(in);
    const char table[]="0123456789abcdef";
    register uint32_t value;
    for(;*in;in++){
        if(*in<127){
            if(*in==0x20){//空格变+符号
                *(out++)='+';
                continue;
            }
            if( (*in>='0' && *in<='9') || (*in>='a' && *in<='z') || (*in>='A' && *in<='Z') || *in=='.' || *in=='-' || *in=='_' ){
                *(out++)=*in;
            }else{
                *(out++)='%';
                value=( (*in) & 255 );
                *(++out)=table[value&0xf];
                value=(value>>4);//value/=16;
                *(--out)=table[value&0xf];
                out+=2;
            }
        }else{
            *(out++)='%';
            value=( (*in) & 255 );
            *(++out)=table[value&0xf];
            value=(value>>4);//value/=16;
            *(--out)=table[value&0xf];
            out+=2;
            ++in;
            *(out++)='%';
            value=( (*in) & 255 );
            *(++out)=table[value&0xf];
            value=(value>>4);//value/=16;
            *(--out)=table[value&0xf];
            out+=2;
            ++in;
        }
    }
    *out=0;
}

string tools::CStringTools::UrlEncodeForFql(const string &str)
{
    std::string strTmp = str;
    std::size_t len = strTmp.length();
    cl_ptr<char> ptrStr((char*)malloc(sizeof(char) * 3 * len));
    if (NULL == ptrStr.get())
    {
        return "";
    }

    urlencode((char*)strTmp.c_str(), ptrStr.get());

    return std::string(ptrStr.get());
}

std::string tools::CStringTools::UrlParamEncode(std::string strUrl)
{
    std::string strResult;
    for(size_t index = 0; index < strUrl.size(); index++)
    {
        char buf[5];
        memset( buf, 0, 5 );
        if (isalnum((char)strUrl[index]))
        {
           buf[0] = strUrl[index];
        }
        else
        {
           buf[0] = '%';
           buf[1] = toHex( (char)strUrl[index] >> 4 );
           buf[2] = toHex( (char)strUrl[index] % 16);
        }
        strResult += (char *)buf;
    }
    return strResult;
}


//URL 编码
std::string tools::CStringTools::UrlEncodeNew(const std::string& szToEncode)
 {
    std::string src = szToEncode;
 char hex[] = "0123456789ABCDEF";
 string dst;
 for (size_t i = 0; i < src.size(); ++i)
 {
   unsigned char cc = src[i];
   if (isascii(cc))
   {
    if (cc == ' ')
    {
     dst += "%20";
      }
    else
    dst += cc;
   }
   else
  {
      unsigned char c = static_cast<unsigned char>(src[i]);
    dst += '%';
    dst += hex[c / 16];
    dst += hex[c % 16];
   }
    }
 return dst;
 }

std::string tools::CStringTools::UrlEncodeForSendMsm(const std::string& str)
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (isalnum((unsigned char)str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            strTemp += toHex((unsigned char)str[i] >> 4);
            strTemp += toHex((unsigned char)str[i] % 16);
        }
    }
    return strTemp;
}

//URL 解码
std::string tools::CStringTools::UrlDecodeNew(const std::string& szToDecode)
 {
 std::string result;
 int hex = 0;
 for (size_t i = 0; i < szToDecode.length(); ++i)
 {
   switch (szToDecode[i])
   {
   case '+':
    result += ' ';
    break;
   case '%':
    if (isxdigit(szToDecode[i + 1]) && isxdigit(szToDecode[i + 2]))
    {
     std::string hexStr = szToDecode.substr(i + 1, 2);
     hex = strtol(hexStr.c_str(), 0, 16);
     //字母和数字[0-9a-zA-Z]、一些特殊符号[$-_.+!*'(),] 、以及某些保留字[$&+,/:;=?@]
       //可以不经过编码直接用于URL
     if (!((hex >= 48 && hex <= 57) || //0-9
      (hex >=97 && hex <= 122) ||   //a-z
      (hex >=65 && hex <= 90) ||    //A-Z
      //一些特殊符号及保留字[$-_.+!*'(),]  [$&+,/:;=?@]
      hex == 0x21 || hex == 0x24 || hex == 0x26 || hex == 0x27 || hex == 0x28 || hex == 0x29
     || hex == 0x2a || hex == 0x2b|| hex == 0x2c || hex == 0x2d || hex == 0x2e || hex == 0x2f
     || hex == 0x3A || hex == 0x3B|| hex == 0x3D || hex == 0x3f || hex == 0x40 || hex == 0x5f
     ))
      {
       result += char(hex);
       i += 2;
      }
      else result += '%';
      }else {
        result += '%';
       }
      break;
      default:
     result += szToDecode[i];
     break;
   }
    }
 return result;
 }


bool tools::CStringTools::IsHostName(const string& strUrl)
{
    const string strHostName = "huishoubao.com.cn";
    size_t last = 0;
    size_t uBeginPos = strUrl.find_first_of(".", last);
    if (string::npos == uBeginPos)
    {
        return false;
    }

    last = uBeginPos;
    size_t uEndPos = strUrl.find_first_of("/", last);
    if (string::npos == uEndPos)
    {
        return false;
    }

    string strResult = strUrl.substr(uBeginPos + 1, uEndPos - uBeginPos - 1);

    if (strHostName != strResult)
    {
        return false;
    }
    return true;
}

string tools::CStringTools:: StrReplace(const string &strInput, const string &strSrc, const string &strDec)
{
    string strtmp = strInput;
    string::size_type fpos = 0;
    int len = strDec.size() - strSrc.size() + 1;

    fpos = strtmp.find(strSrc, fpos);
    while( strtmp.npos !=  fpos)
    {
        strtmp.replace( fpos, strSrc.size(), strDec );
        fpos = strtmp.find( strSrc, fpos+len);
    }
    return strtmp;
}


string tools::CStringTools:: StrGetSubString(const string &strInput, const string &strFindStart, const string &strFindEnd)
{
    string strtmp = strInput, strSubstr;
    string::size_type fpos = 0, spos = 0;
    fpos = strtmp.find(strFindStart, spos);
    string::size_type sLen = strFindStart.size();
    while (strtmp.npos != fpos)
    {
        if (strtmp.npos == fpos)
        {
            break;
        }
        spos = fpos + sLen;
        fpos = strtmp.find(strFindEnd, spos);
        if (strtmp.npos == fpos)
        {
           break;
        }
        strSubstr += strtmp.substr(spos, fpos-spos) + " ";
        spos = fpos + strFindEnd.size();
        fpos = strtmp.find(strFindStart, spos);
    }

    return strSubstr;
}


string tools::CStringTools::VectorToString(const vector< unsigned int >& vecInput)
{
    if (vecInput.empty())
    {
        return string("");
    }
    stringstream ssTmp;
    ssTmp << vecInput[0];
    for (size_t iIndex = 1; iIndex < vecInput.size(); ++iIndex)
    {
        ssTmp << "#" << vecInput[iIndex];
    }
    return ssTmp.str();
}

string tools::CStringTools::VectorToString(const vector< unsigned int >& vecInput, const string& strSeparator)
{
    if (vecInput.empty())
    {
        return string("");
    }
    stringstream ssTmp;
    ssTmp << vecInput[0];
    for (size_t iIndex = 1; iIndex < vecInput.size(); ++iIndex)
    {
        ssTmp << strSeparator << vecInput[iIndex];
    }
    return ssTmp.str();
}

string tools::CStringTools::VectorToString(const vector<string>& vecInput, const string& strSeparator)
{
    if (vecInput.empty())
    {
        return string("");
    }
    stringstream ssTmp;
    ssTmp << vecInput[0];
    for (size_t iIndex = 1; iIndex < vecInput.size(); ++iIndex)
    {
        ssTmp << strSeparator << vecInput[iIndex];
    }
    return ssTmp.str();
}


string tools::CStringTools::IntVectorToString(const vector< unsigned int >& vecInput, const string& strSeparator)
{
    if (vecInput.empty())
    {
        return string("");
    }
    stringstream ssTmp;
    ssTmp << vecInput[0];
    for (size_t iIndex = 1; iIndex < vecInput.size(); ++iIndex)
    {
        ssTmp << strSeparator << vecInput[iIndex];
    }
    return ssTmp.str();
}


bool tools::CStringTools::IsPhoneNum(const string& strPhone)
{
    // 11位
    if (11 != strPhone.length())
    {
        return false;
    }

    // 1开头
    if ('1' != strPhone[0])
    {
        return false;
    }

    // 全是数字
    for (unsigned int i = 0; i < strPhone.length(); ++i)
    {
        if ('0' > strPhone[i] || '9' < strPhone[i])
        {
            return false;
        }
    }

    return true;
}


string tools::CStringTools::GetRandStr(unsigned int iLength)
{
    ::srand((unsigned int)time(NULL));
    char* pRand = new char[iLength + 1];
    memset(pRand, 0, iLength + 1);

    for(unsigned int i = 0; i < iLength; ++i)
        pRand[i] = 'A' + rand()%26;
    pRand[iLength + 1]='\0';

    string strRand = pRand;
    delete pRand;

    return strRand;
}

string tools::CStringTools::vectorToString(const vector<string>& vecInput, const string& strSeparator)
{
    if (vecInput.empty())
    {
        return string("");
    }

    string strTmp = vecInput[0];
    size_t maxSize = vecInput.size();
    for (size_t iIndex = 1; iIndex < maxSize; iIndex++)
    {
        strTmp += strSeparator + vecInput[iIndex];
    }

    return strTmp;
}

string tools::CStringTools::FormatPrice(unsigned int iPrice)
{
    float price = (float)iPrice/100;
    char cTmp[16] = "";
    sprintf_s(cTmp, "%.2f", price);
    return string(cTmp);
}


string tools::CStringTools::FormatClearSpacing(const string strInput, unsigned int iFlag)
{
    string strTmp = strInput;
    string::size_type fpos = 0;
    fpos = strTmp.find(" ", 0);
    while (fpos != strTmp.npos)
    {
        if ((0 == fpos || (strTmp.size()-1) == fpos) && 2 == iFlag)
        {
            fpos = strTmp.find(" ", fpos+1);
            continue;
        }
        if (1 == iFlag && fpos > 0 && fpos < (strTmp.size()-1))
        {
            fpos = strTmp.find(" ", fpos+1);
            continue;
        }

        strTmp.replace(fpos, 1, "");
        fpos = strTmp.find(" ", 0);
    }
    return strTmp;
}


string tools::CStringTools::FormatBarcode(const string strInput)
{
    string strTmp;
    if (strInput.size() < 22)
    {
        strTmp = strInput;
    }
    else if (string("0000") == strInput.substr(18))
    {
        strTmp = strInput.substr(0,7) + "-" + strInput.substr(7,5) + "-" + strInput.substr(12);
    }
    else
    {
        strTmp = strInput.substr(0,17) + "-" + strInput.substr(17);
    }
    return strTmp;
}


bool tools::CStringTools::IsStringAllNum(const string& strNum)
{
    // 全是数字
    std::size_t len = strNum.length();
    for (std::size_t i = 0; i < len; ++i)
    {
        if (!::isdigit((int)strNum[i]))
        {
            return false;
        }
    }

    return true;
}

bool tools::CStringTools::IsStringAllSupper(const std::string& strNum)
{
    std::size_t len = strNum.length();
    for (std::size_t i = 0; i < len; ++i)
    {
        if (!::isupper((int)strNum[i]))
        {
            return false;
        }
    }

    return true;
}


int tools::CStringTools::ToUpper(int c)
{
    return toupper(c);
}

void tools::CStringTools::ToUpperString(string &str)
{
    std::transform(str.begin(), str.end(), str.begin(), ToUpper);
}

int tools::CStringTools::ToLower(int c)
{
    return tolower(c);
}

void tools::CStringTools::ToLowerString(string &str)
{
    std::transform(str.begin(), str.end(), str.begin(), ToLower);
}

//得到自排序签名串(按照ASCII升序)
void tools::CStringTools::GetAutoSortSignString(
        std::map<std::string, std::string> & mapSourceData,
        const std::string &strFirstSep,	//分离键值对
		const std::string &strSecondSep,	//分离键和值
        std::string& strSign)
{
    std::map<std::string, std::string>::const_iterator ite_beg = mapSourceData.begin();

    for(;ite_beg != mapSourceData.end(); ite_beg++ )
    {
        strSign += strFirstSep + ite_beg->first + strSecondSep + ite_beg->second;
    }

    strSign = strSign.substr(1);

}


string tools::CStringTools::Float2StringFixedPoint(double dInput, int nDecimal)
{
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    stringstream ssFormat;
    ssFormat << "%." << nDecimal << "lf";

    sprintf_s(buffer, ssFormat.str().c_str(), dInput);

    return std::string(buffer);

}

std::string tools::CStringTools::string_to_hex(const std::string& input)
{

	static const char* const lut = "0123456789ABCDEF";
	size_t len = input.length();

	std::string output;
	output.reserve(2 * len);
	for (size_t i = 0; i < len; ++i)
	{
		const unsigned char c = input[i];
		output.push_back(lut[c >> 4]);
		output.push_back(lut[c & 15]);
	}

	return output;
}

std::string tools::CStringTools::hex_to_string(const std::string& input)
{
	static const char* const lut = "0123456789ABCDEF";
	size_t len = input.length();
	if (len & 1) throw std::invalid_argument("odd length");

	std::string output;
	output.reserve(len / 2);
	for (size_t i = 0; i < len; i += 2)
	{
		char a = input[i];
		const char* p = std::lower_bound(lut, lut + 16, a);
		if (*p != a) throw std::invalid_argument("not a hex digit");

		char b = input[i + 1];
		const char* q = std::lower_bound(lut, lut + 16, b);
		if (*q != b) throw std::invalid_argument("not a hex digit");

		output.push_back(((p - lut) << 4) | (q - lut));
	}
	return output;
}
