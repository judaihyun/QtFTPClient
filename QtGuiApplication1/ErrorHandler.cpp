#include "ErrorHandler.h"
#include <tchar.h>

#define PRINT_BUFSIZE 10000




void ErrorHandler::err_quit(const char * msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	//MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void ErrorHandler::err_display(const char* msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
//	DisplayText("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}



void ErrorHandler::setLogLevel(logLevel level) {
	lev = level;
}

/*
void ErrorHandler::DisplayDir(const char *fmt, ...) {
va_list arg;
va_start(arg, fmt);
char cbuf[PRINT_BUFSIZE];

vsprintf_s(cbuf, PRINT_BUFSIZE, fmt, arg);
int nLength = GetWindowTextLength(dirEdit);
SendMessage(dirEdit, EM_SETSEL, nLength, nLength);
SendMessage(dirEdit, WM_SETTEXT, NULL, NULL);
SendMessage(dirEdit, EM_REPLACESEL, true, (LPARAM)cbuf);


va_end(arg);
}
*/