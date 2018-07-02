#pragma once
#include "WinSockHeader.h"
#include "ErrorHandler.h"
#include "Utils.h"
#include <qobject.h>
#include <qeventloop.h>
#include <qtextstream.h>
#define COM_BUFSIZE 1024  // command for request
#define RES_BUFSIZE 1024  // command for response
#define DIR_BUFSIZE 1000000
#define MAX_FILE_SIZE 1000000
#define MTU_FILE 4096



class ControlHandler : public QObject {
	Q_OBJECT
signals :
	void printLog(logLevel, const QString&);
	void printDirCwd(const QString&);
	void recurDirList(const QString&);
protected slots :
void commandDetect(const QString& input);
void exitProcedure() {
	loop.exit();
}
public:
	ControlHandler(passToThread arg, QObject* p) : argList{ arg }  {
		parent = p;
		connect(parent, SIGNAL(sendCommand(const QString&)), this, SLOT(commandDetect(const QString&))); // received command from UI
		connect(parent, SIGNAL(exitCommand()), this, SLOT(exitProcedure()));  // QEventLoop Exit
		connect(this, SIGNAL(recurDirList(const QString&)), parent, SLOT(recurDirList(const QString&)));
		connect(this, SIGNAL(printDirCwd(const QString&)), parent, SLOT(printDirCwd(const QString&)));
		connect(this, SIGNAL(printLog(logLevel, const QString&)), parent, SLOT(ftpLog(logLevel, QString)));  // Log
	//	connect(this, SIGNAL(printDir(const QString&)), parent, SLOT(printDirGui(QString)));  // Log
		//strcpy_s(requestBuf, sizeof(requestBuf), "PASV_INIT");
		controlSock = (SOCKET)argList.sock;
		defaultPort = argList.controlPort;
		serverIP = argList.serverIP;
	}
	~ControlHandler() {
		emit printLog(LOG_INFO, "ControlHandler exit");
		closesocket(controlSock);
		closesocket(dataConnectSock);
		closesocket(clientDataSock);
	}
	int getConId() { return argList.id; }
	int getFileList();
	void setFileName(string n) { fileName = n; };
	string getFileName() { return fileName; }
	__int64 openFile();
	void createPASVSock(string); // passive
	void createPORTSock(string); // active
	string getRootPath() { if (rootPath == "") { errorHandle->err_quit("ControlHandler() root path"); } return rootPath; }
	string getCurPath() { if (curPath == "") { emit printLog(LOG_ERROR, "path not set"); }return curPath; }
	void addPath(string p) { curPath += p; }  // for CWD
	void movePath(string p) { curPath = p; }  // for CWD
	int controlActivate();
	int sendMsg(const string);
	int pasvRecvFile();
	int portSendFile();
	void setRetrSize(__int64 r) { sizeRETR = r; };
	__int64 getRetrSize() { return sizeRETR; };
	void pasvSendList();
	void portSendList();
	void pasvInitProcess();
	int responseHandler();
	int requestHandler();
	void controlRecv();
	void setActivePort(int ap) { activePort = ap; }
	int getActivePort() { return activePort; };
	string getServerIp() { return serverIP; }
private:
	QObject * parent;
	QEventLoop loop;
	bool requestedStop = false;
	passToThread argList;
	const string rootPath = argList.rootPath;  //set by main
	int activePort{ 0 };
	int defaultPort{ 0 };
	string serverIP{ "0.0.0.0" };
	string curPath{ "/" };  //current path
	string SP{ " " }; //space
	string CRLF{ "\r\n" };
	string fileName{ "" };
	char requestBuf[COM_BUFSIZE];
	__int64 sizeRETR{ 0 };
	bool isActive = false;
	char responseBuf[COM_BUFSIZE]{ '\0' };
	char resBuf[RES_BUFSIZE + 1]{ "220 FTP Test Serivce. \r\n" };
	char dirList[DIR_BUFSIZE + 1];
	char buf[MAX_FILE_SIZE];

	SOCKET controlSock = INVALID_SOCKET;
	SOCKET dataConnectSock = INVALID_SOCKET; // for PASV
	SOCKET clientDataSock = INVALID_SOCKET;

	SOCKADDR_IN controlClient_addr;
	SOCKADDR_IN dataServerAddr; // for PASV
	SOCKADDR_IN dataClient_addr; // for PASV
	SOCKADDR_IN dataCon_addr;    // for Active mode

	ifstream *ifs;
	ErrorHandler *errorHandle;


};

