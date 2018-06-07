#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <filesystem>
#include <process.h>
#include <Ws2def.h>
#include <Wsrm.h>
#include <iomanip>

//#include "ErrorControl.h"
//#include "ErrorHandler.h"


using namespace std;
namespace fs = std::experimental::filesystem;

struct passToThread {
	void* sock;
	int activePort{ 0 };
	int controlPort{ 0 };
	string serverIP{ "" };
	string rootPath;
	int id{ 0 };
};
enum logLevel {
	LOG_FETAL = 1, LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG, LOG_TRACE
};
