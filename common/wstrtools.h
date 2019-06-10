#ifndef _WSTR_TOOLS_H_
#define _WSTR_TOOLS_H_


#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdexcept>



namespace tools
{
	using std::map;
	using std::vector;
	using std::string;
	using std::wstring;
	using std::stringstream;


	class CWstrTools
	{

	public:

		static string wstr2str(const wstring &ws);
	};

};



#endif //_WSTR_TOOLS_H_