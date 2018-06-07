#pragma once
#include "WinSockHeader.h"
#include <Windows.h>
#include <iostream>
//#include "QtGuiApplication1.h"
using namespace std;

class ErrorHandler{
private:

public:
	void setLogLevel(logLevel level);
	void err_display(const char* msg);
	void err_quit(const char * msg);
	logLevel lev;



};

