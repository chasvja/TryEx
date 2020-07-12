# TryEx
 Tool for {C++11's 'try-catch'} and {MSVC‘s '_TRY-_except'}

This is a tool that facilitates the use of the'try-catch' keyword, 
which is used to catch exceptions, reduce the need to write a large number of try{}catch{}, and includes the _TRY _except macro in Microsoft MSVC


Usage, such as:

#include "TryEX.h"

auto trsp = TryEx::Run([](CMainProcess *process, char *strIn, char *strOut) -> int {
	return process->Dispatch(strIn, strOut);
},&process, szIn, szOut);

if (trsp.ExMsg.empty())
{
	nRspCode = trsp.v;
}
else
{
	nRspCode = -1;
	WRITELOG(trsp.ExMsg.c_str());
	MessageBox(NULL, trsp.ExMsg.c_str(), "Exception", MB_OK);
}







