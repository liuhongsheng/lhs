#ifndef _STRING_TOOLS_H_
#define _STRING_TOOLS_H_

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdexcept>

using namespace std;
namespace tools
{

class CStringTools
{
public:

    static void Show(const std::vector<unsigned int>& vecInput);
    static void Show(const std::map<std::string, std::string>& mapInput);

	static std::string Float2String(double fInput, int nplaces = 3);
    static std::string Int2String(int iInput);
    static std::string Int2String(unsigned int iInput);
    static std::string Int2String(unsigned long iInput);

    static std::vector<std::string> Split2String(const std::string& strInput, const std::string& strSeparator, bool returnIncludeEmptyStr=false);
    static std::vector<unsigned int> Split2Uint(const std::string& strInput, const std::string& strSeparator);
    static std::vector<int> Split2Int(const std::string& strInput, const std::string& strSeparator);

    static std::map<std::string, std::string> SplitTwice2String(const std::string& strInput, const std::string& strSeparator, const std::string& strSeparator2);
    static std::map<unsigned int, unsigned int> SplitTwice2Uint(const std::string& strInput, const std::string& strSeparator, const std::string& strSeparator2);

    static std::string Trim(const std::string& strInput);
    static std::string TrimTab(const std::string& strInput);
    static std::string TrimLR(const std::string & strInput);
    static std::string TrimLRReal(const std::string & strInput);

    static std::string UrlDecode(const std::string& strUrl);
    static std::string UrlEncode(const std::string strUrl);
    static std::string UrlParamEncode(const std::string strUrl);
    static std::string UrlEncodeNew(const std::string& strUrl);
    static std::string UrlEncodeForSendMsm(const std::string& strUrl);
    static std::string UrlDecodeNew(const std::string& strUrl);
    static std::string UrlEncodeForFql(const std::string &str);


    static std::string StrReplace(const std::string &strInput, const std::string &strSrc, const std::string &strDec);
    static std::string StrGetSubString(const std::string &strInput, const std::string &strFindStart, const std::string &strFindEnd);

    static std::string VectorToString( const std::vector< unsigned int >& vecInput);
    static std::string VectorToString( const std::vector< unsigned int >& vecInput,const string& strSperator);
    static std::string VectorToString( const std::vector< std::string >& vecInput, const string& strSeparator);

    static bool IsHostName(const std::string & strUrl);
    static bool IsPhoneNum(const std::string& strPhone);


    static string IntVectorToString(const vector< unsigned int >& vecInput, const string& strSeparator);
    static std::string vectorToString(const std::vector<std::string>& vecInput, const std::string& strSeparator);

    static std::string FormatPrice(unsigned int iPrice);
    static std::string FormatClearSpacing(const std::string strInput, unsigned int iFlag = 0);
    static std::string FormatBarcode(const std::string strInput);

    static bool IsStringAllNum(const std::string& strNum);
    static bool IsStringAllSupper(const std::string& strNum);


    template <class Type>
    static std::map<Type, unsigned int> Vector2Map(std::vector<Type> vecInput)
    {
        std::map<Type, unsigned int> mapResult;
        size_t maxSize = vecInput.size();
        for (size_t index = 0; index < maxSize; index++)
        {
            mapResult.insert(std::make_pair<Type, unsigned int>(vecInput[index], 1));
        }
        return mapResult;
    }


    template <class Type>
    static Type StringToNum(std::string& strInput)
    {
        std::istringstream ssInput(strInput);
        Type num;
        ssInput >> num;
        return num;
    }

    static std::string GetRandStr(unsigned int iLength);

	static int ToUpper(int c);
	static void ToUpperString(std::string &str);
    
    static int ToLower(int c);
	static void ToLowerString(std::string &str);

	//得到自动排序的签名串(按照ASCII升序)
    static void GetAutoSortSignString(std::map<std::string, std::string> & mapSourceData,
                                      const std::string &strFirstSep,
                                      const std::string &strSecondSep,
                                      std::string& strSign);

    static std::string Float2StringFixedPoint(double dInput, int nDecimal);


	static std::string string_to_hex(const std::string& input);

	static std::string hex_to_string(const std::string& input);


};

template<typename T>
class cl_ptr
{
public:
    typedef void (*pFreeFunc)(void*);

    explicit cl_ptr(T* ptr):m_ptr(ptr),m_pfree(::free){}
    explicit cl_ptr(T* ptr, pFreeFunc pfree):m_ptr(ptr),m_pfree(pfree){}

    ~cl_ptr()
    {
        if (m_ptr)
        {
            m_pfree(m_ptr);
        }
    }

    T* operator->() const
    {
        return m_ptr;
    }

    T* get() const
    {
        return m_ptr;
    }

private:
    cl_ptr(const cl_ptr&);
    cl_ptr& operator=(const cl_ptr&);

    T* m_ptr;
    pFreeFunc m_pfree;
};


}

#endif
