#include <iostream>
#include "http.h"

using namespace std;


int doMainProc()
{
	protocol::CRequest oCRequest(L"https://www.baidu.com/");

	protocol::CHttp http;
	http.SendRequest(oCRequest.CreateRequest());
	printf("%s", (char*)http.GetResponseData());
	return 0;
}


int main(int argc, char *argv[])
{
	doMainProc();
	system("pause");
	return 0;
}