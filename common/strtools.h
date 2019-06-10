#ifndef _STR_TOOLS_H_
#define _STR_TOOLS_H_

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdexcept>



namespace tools
{
	using std::map;
	using std::string;
	using std::vector;
	using std::stringstream;


	class CStrTools
	{

	public:

		static string float2string(double in, int nplaces = 3);
		static string double2string(double in, int nDecimal);
		
		template<typename T>
		static string int2string(T in)
		{
			stringstream oss;
			oss << in;
			return oss.str();
		}


		static vector<int> Split2Int(const string& strInput, const string& strSeparator);
		static vector<unsigned int> Split2Uint(const string& strInput, const string& strSeparator);
		static vector<string> Split2String(const string& strInput, const string& strSeparator, bool returnIncludeEmptyStr = false);

	
		static map<string, string> SplitTwice2String(const string& strInput, const string& strSep1, const string& strSep2);
		static map<unsigned int, unsigned int> SplitTwice2Uint(const string& strInput, const string& strSeparator, const string& strSeparator2);


		static string TrimSpace(const string& strInput);
		static string TrimTab(const string& strInput);
		static string TrimReal(const string & strInput);
		static string TrimLR(const string & strInput);
		static string TrimLRReal(const string & strInput);


		static string UrlDecode(const string& strUrl);
		static string UrlEncode(const string strUrl);
		static string UrlParamEncode(const string strUrl);
		static string UrlEncodeNew(const string& strUrl);
		static string UrlDecodeNew(const string& strUrl);


		static string StrReplace(const string &strInput, const string &strSrc, const string &strDec);
		static string StrGetSubString(const string &strInput, const string &strFindStart, const string &strFindEnd);

		static string VectorToString(const vector< unsigned int >& vecInput);
		static string VectorToString(const vector< unsigned int >& vecInput, const string& strSperator);
		static string VectorToString(const vector< string >& vecInput, const string& strSeparator);

		static bool IsHostName(const string & strUrl, const string strHostName);
		static bool IsPhoneNum(const string& strPhone);


		static string IntVectorToString(const vector< unsigned int >& vecInput, const string& strSeparator);
		static string vectorToString(const vector<string>& vecInput, const string& strSeparator);


		static string FormatClearSpacing(const string strInput, unsigned int iFlag = 0);
		static string FormatBarcode(const string strInput);


		static bool IsStringAllNum(const string& strNum);
		static bool IsStringAllSupper(const string& strNum);


		static string GetRandStr(unsigned int iLength);


		static int ToUpper(int c);
		static void ToUpperString(string &str);


		static int ToLower(int c);
		static void ToLowerString(string &str);


		//得到自动排序的签名串(按照ASCII升序)
		static void GetAutoSortSignString(map<string, string> & mapSourceData,
			const string &strFirstSep,
			const string &strSecondSep,
			string& strSign);



		static string string2hex(const string& input);
		static string hex2string(const string& input);


		static std::wstring str2wstr(const string &s);

		template <class T>
		static map<T, unsigned int> vector2map(vector<T> vecInput)
		{
			map<T, unsigned int> mapResult;
			size_t maxSize = vecInput.size();
			for (size_t index = 0; index < maxSize; index++)
			{
				mapResult.insert(make_pair<T, unsigned int>(vecInput[index], 1));
			}

			return mapResult;
		}


		template <typename T>
		static T string2num(string& strInput)
		{
			istringstream oss(strInput);
			T num;
			oss >> num;
			return num;
		}

		template<typename T>
		static void Show(const vector<T>& vecInput)
		{
			stringstream oss;
			for (auto &&v : vecInput)
			{
				oss.str("");
				oss.clear();
				oss << v;
				cout << oss.str().c_str() << end;
			}
		}

		template<typename T1, typename T2>
		static void Show(const map<T1, T2>& mapInput)
		{
			stringstream oss;
			for (auto &&v : mapInput)
			{
				oss.str("");
				oss.clear();
				oss << v.first << " : " << v.second;
				cout << oss.str().c_str() << endl;
			}
		}

	};

};


#endif // _STR_TOOLS_H_
