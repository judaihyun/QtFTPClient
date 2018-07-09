#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include "WinSockHeader.h"
#include "ControlHandler.h"
#include "ErrorHandler.h"
#include <qthread.h>
#include "Utils.h"

class FtpClient : public QObject{
	Q_OBJECT
signals :
	void printLog(logLevel, const QString&);
	void exitCommand();
public slots:
	void starter();
public:
	~FtpClient(){
		emit printLog(LOG_DEBUG, "FtpClient() Destructed");
		if(controlHandler != nullptr) delete controlHandler;
		cout << "~FtpClient\n";
		closesocket(controlSock);
	};
	FtpClient(void* arg, QObject* p) : argList{ *((passToThread*)arg) } {
		parent = p;
		serverIP = argList.serverIP;
		controlPort = argList.controlPort;
		connect(this, SIGNAL(exitCommand()), parent, SLOT(connectFailed())); 
		connect(this, SIGNAL(printLog(logLevel, const QString&)), parent, SLOT(ftpLog(logLevel, QString)));  
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) exit(1);
	};

	void setRemotePath(string r) {
		if (!r.empty()) {
			argList.rootPath = r;
			setOrNot = true;
		}
	};
	void setLocalPath(string r) {
		if (!r.empty()) {
			argList.localRoot = r;
		}
	}

	void setActivePort(int ap) { activePort = ap; }
	int getActivePort() { return activePort; }
	void setControlPort(int p) { controlPort = p; }
	int getControlPort() { return controlPort; }

	void setSecureFTP(bool i) { secureFTP = i; };
	void connecting();
	//void connecting(SOCKET&);
private:
	ControlHandler * controlHandler = nullptr;
	QObject * parent;

	WSADATA wsa;
	SOCKET controlSock = INVALID_SOCKET;
	SOCKADDR_IN controlAddr;

	int controlPort{ 210 };
	int activePort{ 200 };
	//string serverIP{ "192.168.219.145" };
	string serverIP{ "0.0.0.0" };

	bool setOrNot = false;
	HANDLE fThread1;
	passToThread argList;
	ErrorHandler * errorHandler;
	bool secureFTP = NULL;
};



