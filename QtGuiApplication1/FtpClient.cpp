#include "FtpClient.h"
#include "ErrorHandler.h"

void FtpClient::starter/*SLOT*/()
{
	
	char buf[1000];
	int retval{ 0 };
	if (setOrNot == false) {
		errorHandler->err_quit("Path Not Set");
		emit printLog(LOG_ERROR, "Path Not Set");
		return;
	}
	//socket
	controlSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (controlSock == INVALID_SOCKET) errorHandler->err_quit("socket()");
	ZeroMemory(&controlAddr, sizeof(controlAddr));
	controlAddr.sin_family = AF_INET;
	controlAddr.sin_addr.S_un.S_addr = inet_addr(argList.serverIP.c_str());
	controlAddr.sin_port = htons(controlPort);
	
	emit printLog(LOG_INFO, "==================================================");
	emit printLog(LOG_INFO, variadicToQstring("[Client - ControlChannel] : IP = %s, Port = %d",
		inet_ntop(AF_INET, &controlAddr.sin_addr, buf, sizeof(buf)), ntohs(controlAddr.sin_port)));

	connecting();
}

//void FtpClient::connecting(SOCKET &controlSock) {
void FtpClient::connecting() {
	int addrlen{ 0 };
	int retval{ 0 };
	retval = ::connect(controlSock, (SOCKADDR*)&controlAddr, sizeof(controlAddr));
	if (retval == SOCKET_ERROR) {
		emit printLog(LOG_ERROR, variadicToQstring("connect error. [%d]", WSAGetLastError()));
		emit exitCommand();
		return;
	}


	emit printLog(LOG_INFO, variadicToQstring("[connected to ...]  IP=%s, Port=%d", 
		inet_ntoa(controlAddr.sin_addr), ntohs(controlAddr.sin_port)));
	emit printLog(LOG_INFO, "==================================================");

	// pass to ControlHandler class
	argList.sock = (void*)controlSock; //with root path


	controlHandler = new ControlHandler(argList, parent);
	controlHandler->controlActivate();
}
