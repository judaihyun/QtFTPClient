#pragma once
#include "WinSockHeader.h"
#include <Windows.h>
#include <iostream>
#include <qstring.h>
//#include "QtGuiApplication1.h"
using namespace std;

class ErrorHandler{
private:

public:
	void err_display(const char* msg);
	void err_quit(const char * msg);

};

