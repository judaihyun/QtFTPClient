#include "ControlHandler.h"

namespace qcout {
	QTextStream cout(stdout, QIODevice::WriteOnly);
}

void ControlHandler::controlRecv() {
	int retval{ 0 };
	responseBuf[0] = '\0';
	retval = recv(controlSock, responseBuf, RES_BUFSIZE, 0);
	responseBuf[retval] = '\0';
	if (retval == SOCKET_ERROR) {
		errorHandle->err_display("recv()");
	}
	else if (retval == 0) {
	}

}

int ControlHandler::sendMsg(const string msg) {
	emit printLog(LOG_DEBUG, variadicToQstring("SENDMSG: %s", msg.c_str()));
	int retval = send(controlSock, msg.c_str(), msg.length(), 0);
	if (retval == SOCKET_ERROR) {
		errorHandle->err_display("sendMsg()");

	}
	
	return retval;
}



void ControlHandler::commandDetect(const QString& input) {
	QByteArray commandMsg = toUniString(input);
	
	
	emit printLog(LOG_TRACE, "input commands " + commandMsg);
	
	
	//strcpy_s(requestBuf, sizeof(requestBuf), commandMsg);
	strcpy_s(requestBuf, sizeof(requestBuf), commandMsg.left(commandMsg.length() - 1).toStdString().c_str());

	requestHandler();
};

int ControlHandler::controlActivate() {
	emit printLog(LOG_INFO, variadicToQstring("%d, ControlActivated", getConId()));
	int addrlen{ 0 };
	int retval{ 0 };
	

	// if PASV or PORT 해야함

	
	pasvInitProcess();
	loop.exec();
	cout << "loop exit() in controlHandler\n";

	return 0;
}


int ControlHandler::requestHandler() {

	int retval{ 0 };
	vector<string> argv;

	commandSeparator(argv, requestBuf);
	
	cout << "request commands : " << argv[0] << " " << argv[1] << endl;

	
	if (argv[0] == "LIST") {
		sendMsg("PASV");
		controlRecv();
		responseHandler();

		sendMsg("LIST");
		retval = recv(dataConnectSock, dirList, DIR_BUFSIZE, 0);
		if (retval == SOCKET_ERROR) errorHandle->err_display("recv()");

		emit printDirCwd(QString::fromLocal8Bit(dirList));

		controlRecv();
		responseHandler();
	}
	else if (argv[0] == "CWDLIST") { /* get specific Directory List for downloading list*/
		sendMsg("CWD " + argv[1] + CRLF);
		controlRecv();
		responseHandler();
		
		sendMsg("PWD");
		controlRecv();
		responseHandler();

		sendMsg("PASV");
		controlRecv();
		responseHandler();

		sendMsg("LIST");
		controlRecv();
		responseHandler();

		retval = recv(dataConnectSock, dirList, DIR_BUFSIZE, 0);
		if (retval == SOCKET_ERROR) errorHandle->err_display("recv()");
		dirList[retval] = '\0';

		emit recurDirList(QString::fromLocal8Bit(dirList), QString::fromStdString(getCurPath()));
	}
	else if (argv[0] == "CWD") {
		sendMsg("CWD " + argv[1] + CRLF);
		controlRecv();
		responseHandler();

		setFilePath(argv[1]);

		sendMsg("PWD");
		controlRecv();
		responseHandler();

		sendMsg("PASV");
		controlRecv();
		responseHandler();

		sendMsg("LIST");
		controlRecv();
		responseHandler();

		retval = recv(dataConnectSock, dirList, DIR_BUFSIZE, 0);
		if (retval == SOCKET_ERROR) errorHandle->err_display("recv()");
		dirList[retval] = '\0';
		
		emit printDirCwd(QString::fromLocal8Bit(dirList));
	}
	else if (argv[0] == "BULK") {
		sendMsg("PASV");
		controlRecv();
		responseHandler();

		sendMsg("BULK " + argv[1]);
		controlRecv();
		responseHandler();
	}
	else if (argv[0] == "RETR") {
		string temp{ "" };

		temp = argv[1].substr(argv[1].rfind("/") + 1, argv[1].size());
		setFileName(temp);
		

		if (openFile() < 0) {
			return -1;
		}
		sendMsg("PASV");
		controlRecv();
		responseHandler();

		sendMsg("RETR " + getFileName());

		controlRecv();
		responseHandler();
	
		controlRecv();
		responseHandler();
	}
	else {
		emit printLog(LOG_INFO, variadicToQstring("Not defined command : %s", argv[1].c_str()));
	}

	return 1;
}

int ControlHandler::pasvRecvFile() {
	emit printLog(LOG_DEBUG, "[FUNC] pasvRecvFile()");
	int retval = 0;
	int addrlen{ 0 };
	int size = 4096;
	__int64 fileSize = 0;


	emit printLog(LOG_INFO, variadicToQstring("[PASV.dataChannel - server] : IP = %s, Port = %d\n",
		inet_ntoa(dataServerAddr.sin_addr), ntohs(dataServerAddr.sin_port)));

	int recvSize{ 0 };
	
	while (1) {
		retval = recv(dataConnectSock, buf, size, 0);
		if (retval == SOCKET_ERROR) {
			errorHandle->err_display("send()");
			return -1;
		}
		if (retval == 0) break;
		
		ofs->write(buf, retval);
	
		
	}
	

	ofs->close();
	closesocket(dataConnectSock);
	emit downloadCompleted();
	emit nextToDownload();
	

	return 1;
}



__int64 ControlHandler::openFile() {
	emit printLog(LOG_TRACE, variadicToQstring("openFile(), fileName : %s ", getFileName().c_str())); // 한글깨짐 정상
	__int64 startPos{ 0 };
	__int64 fileSize{ 0 };
	string targetFile = getLocalRoot();
	targetFile += getFilePath() + "/" + getFileName();
	
	cout << "targetFile[" << targetFile << "]" << endl;
	
	fs::create_directories(getLocalRoot() + getFilePath());


	if (checkFileExists(targetFile)) {
		emit printLog(LOG_INFO, variadicToQstring("%s is already exists.", getFileName().c_str()));
		emit downloadResume(); //client 처리 안함
		emit printLog(LOG_TRACE, "wait waitMessage.exec()");
		
		if (getRetrSize() > 0) {  // for REST
			startPos = getRetrSize();
			ifs->seekg(startPos, ios::beg);
			return fileSize;
		}
		return -1;
	}
	else {
		ofs = new ofstream;
		ofs->open(targetFile, std::ios_base::binary);


		if (!ofs) {
			sendMsg("550 file open failed" + CRLF);
			errorHandle->err_quit("file open failed");
			return -1;
		}
	}
	
	return fileSize;
}

int ControlHandler::responseHandler() {
	int retval{ 0 };
	vector<string> argv;

	commandSeparator(argv, responseBuf);
	cout << "response commands : " << argv[0] << " " << argv[1] << endl;

	if (argv[0] == "226") {
		emit printLog(LOG_INFO, variadicToQstring("%s %s", argv[0].c_str(), argv[1].c_str()));
	}
	else if (argv[0] == "227") {    // PASV response
		emit printLog(LOG_INFO, variadicToQstring("%s %s", argv[0].c_str(), argv[1].c_str()));
		createPASVSock(argv[1]);
	}
	else if (argv[0] == "257") { // directory commands
		string path = extractPath(argv[1]);
		movePath(argv[1]);
		emit printLog(LOG_TRACE, variadicToQstring("Current Path : %s", getCurPath().c_str()));
	}
	else if (argv[0] == "LIST") {
		emit printLog(LOG_ERROR, "Must be performed PASV/PORT Command first");
	}
	else if (argv[0] == "250") {   // CWD
		emit printLog(LOG_INFO, variadicToQstring("%s %s",argv[0].c_str(), argv[1].c_str()));
	}
	else if (argv[0] == "220") {  //Service ready for user, welcome
		emit printLog(LOG_INFO, variadicToQstring("%s %s", argv[0].c_str(), argv[1].c_str()));
	}
	else if (argv[0] == "125"){
		emit printLog(LOG_INFO, variadicToQstring("%s %s", argv[0].c_str(), argv[1].c_str()));
		pasvRecvFile();
	}
	else {
		emit printLog(LOG_INFO, variadicToQstring("Not defined response %s", argv[1].c_str()));
	}

	return 1;
}


void ControlHandler::createPASVSock(string argv) {
	emit printLog(LOG_DEBUG, "[FUNC] createPASVSock()");

	int port{ 0 };
	string serverip;
	int retval{ 0 };
	int len{ 0 };
	LINGER linger;
	linger.l_onoff = 1;
	linger.l_linger = 1000;

	portDecoder(argv, serverip, port);
	cout << "port : " << port << ",ip : " << serverip << endl;

	dataConnectSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (dataConnectSock == INVALID_SOCKET) errorHandle->err_quit("data socket()");

	ZeroMemory(&dataServerAddr, sizeof(dataServerAddr));
	dataServerAddr.sin_family = AF_INET;
	dataServerAddr.sin_addr.S_un.S_addr = inet_addr(serverip.c_str());
	//dataServerAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	dataServerAddr.sin_port = htons(port);

	retval = ::connect(dataConnectSock, (SOCKADDR*)&dataServerAddr, sizeof(dataServerAddr));
	if (retval == SOCKET_ERROR) emit printLog(LOG_FETAL, "connect() error");



	emit printLog(LOG_TRACE, "PASV Data Channel Open()");

	isActive = false;
}


int ControlHandler::portSendFile() {
	emit printLog(LOG_INFO, variadicToQstring("%d - [FUNC] portSendFile()", getConId()));
	__int64 retval = 0;
	int addrlen{ 0 };
	int size = 4096;
	__int64 fileSize = 0;
	string name = getFileName();

	fileSize = openFile();
	if (fileSize < 0) {
		emit printLog(LOG_FETAL, "open error()");
		return -1;
	}
	/*
	char buf[4096];
	while (!ifs->eof()) {

		if (!ifs->read(buf, size)) {
			size = (int)ifs->gcount();
		}
		retval = send(clientDataSock, buf, size, 0);
		if (retval == SOCKET_ERROR) {
			errorHandle->err_display("send()");
			return -1;
		}
	}
	*/


	ifs->close();

	shutdown(clientDataSock, SD_SEND);
	retval = closesocket(clientDataSock);
	if (retval == SOCKET_ERROR) {
		errorHandle->err_display("close()");
	}
	/*
	cout << "close socket id : " << clientDataSock << endl;
	cout << "ifs id : " << ifs << ", " << getConId() << " - " << name << " closed\n";
	*/

	sendMsg("226 Transfer complete." + CRLF);

	return 1;
}








void ControlHandler::createPORTSock(string argv) {
	emit printLog(LOG_DEBUG, variadicToQstring("[FUNC]CreatePortSock() = %s ", argv.c_str()));
	string ip{ "" };
	int port{ 0 };
	int retval{ 0 };
	LINGER linger;
	linger.l_onoff = 1;
	linger.l_linger = 1000;

	//portDecoder(argv, ip, port, getServerIp());

	clientDataSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientDataSock == INVALID_SOCKET) errorHandle->err_quit("data con socket()");

	bool optval = true;
	retval = setsockopt(clientDataSock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
	if (retval == SOCKET_ERROR) errorHandle->err_quit("setsockopt() - active");

	retval = setsockopt(clientDataSock, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger));
	if (retval == SOCKET_ERROR) errorHandle->err_quit("setsockopt()");

	ZeroMemory(&dataCon_addr, sizeof(dataCon_addr));   // for connect
	dataCon_addr.sin_family = AF_INET;
	dataCon_addr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	dataCon_addr.sin_port = htons(port);

	ZeroMemory(&dataServerAddr, sizeof(dataServerAddr));  // for bind
	dataServerAddr.sin_family = AF_INET;
	dataServerAddr.sin_addr.S_un.S_addr = inet_addr(getServerIp().c_str());
	//dataServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	dataServerAddr.sin_port = htons(getActivePort());

	retval = ::bind(clientDataSock, (SOCKADDR*)&dataServerAddr, sizeof(dataServerAddr));  //bind
	if (retval == SOCKET_ERROR) errorHandle->err_quit("Active bind()");

	emit printLog(LOG_INFO, variadicToQstring("%d - [dataChannel-(Connecting...)] : IP=%s, Port=%d\n", getConId(),
		inet_ntoa(dataServerAddr.sin_addr), ntohs(dataServerAddr.sin_port)));

	retval = ::connect(clientDataSock, (SOCKADDR*)&dataCon_addr, sizeof(dataCon_addr));   // connect
	if (retval == SOCKET_ERROR) errorHandle->err_quit("Active connect()");
	emit printLog(LOG_INFO, variadicToQstring("%d - [PORT.dataChannel - client] : IP = %s, Port = %d\n", getConId(),
		inet_ntoa(dataCon_addr.sin_addr), ntohs(dataCon_addr.sin_port)));

	isActive = true;

	sendMsg("200 PORT command successful." + CRLF);
}



void ControlHandler::pasvSendList() {
	emit printLog(LOG_DEBUG, "[FUNC]pasvSendList()");
	int retval{ 0 };
	int listLen{ 0 };
	int addrlen{ 0 };

	addrlen = sizeof(dataClient_addr);
	clientDataSock = accept(dataConnectSock, (SOCKADDR*)&dataClient_addr, &addrlen);
	if (clientDataSock == INVALID_SOCKET) {
		errorHandle->err_display("data accept()");
		closesocket(dataConnectSock);
	}
	emit printLog(LOG_INFO, variadicToQstring("[PASV.dataChannel - client] : IP = %s, Port = %d\n",
		inet_ntoa(dataClient_addr.sin_addr), ntohs(dataClient_addr.sin_port)));

	// getList
	listLen = getFileList();
	if (listLen <= 0) {
		emit printLog(LOG_DEBUG, "this dir is empty");
	}
	retval = send(clientDataSock, dirList, strlen(dirList), 0);
	if (retval == SOCKET_ERROR) {
		errorHandle->err_display("send()");
	}

	//	delete[] dirList;
	closesocket(clientDataSock);

	sendMsg("226 Successfully transferred " + getCurPath() + "\"" + CRLF);

	emit printLog(LOG_INFO, variadicToQstring("[dataChannel-client] : IP=%s, Port=%d is closed\n",
		inet_ntoa(dataClient_addr.sin_addr), ntohs(dataClient_addr.sin_port)));
}



void ControlHandler::portSendList() {
	emit printLog(LOG_DEBUG, "[FUNC]portSendList()");

	int retval{ 0 };
	int listLen{ 0 };
	int addrlen{ 0 };

	listLen = getFileList();

	if (listLen <= 0) {
		emit printLog(LOG_DEBUG, "this dir is empty");
	}

	retval = send(clientDataSock, dirList, strlen(dirList), 0);
	if (retval == SOCKET_ERROR) {
		errorHandle->err_display("send()");
	}


	sendMsg("226 Successfully transferred " + getCurPath() + "\"" + CRLF);

	//	delete[] dirList;
	closesocket(clientDataSock);

	emit printLog(LOG_INFO, variadicToQstring("[PORT.dataChannel-client] : IP=%s, Port=%d is closed\n",
		inet_ntoa(dataCon_addr.sin_addr), ntohs(dataCon_addr.sin_port)));
}



int ControlHandler::getFileList() {   // for LIST command
	string targetPath = getRootPath();
	targetPath += getCurPath();

	struct tm ltm;
	//dirList = new char[DIR_BUFSIZE] {""};
	char *tempList = new char[DIR_BUFSIZE];

	emit printLog(LOG_TRACE, variadicToQstring("getFileList() targetPath : [%s]", targetPath.c_str()));

	ostringstream os;
	// 550 access is denied  처리해야함
	try {
		for (auto& p : fs::directory_iterator(targetPath)) {
			if (p.path().filename() == "System Volume Information") continue;
			if (p.path().filename() == "$RECYCLE.BIN") continue;

			auto ftime = fs::last_write_time(p);
			time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
			localtime_s(&ltm, &cftime);
			//printStatus(p, fs::status(p));

			if (p.status().type() == fs::file_type::directory) {
				strftime(tempList, 1000, "%m-%d-%g  %R%p", &ltm);
				os << tempList << " <DIR> " << p.path().filename() << CRLF;
			}
			else if (p.status().type() != fs::file_type::directory) {
				strftime(tempList, 1000, "%m-%d-%g  %R%p", &ltm);
				os << tempList << " " << fs::file_size(p) << " " << p.path().filename() << CRLF;
			}
			else {
				emit printLog(LOG_WARN, "Unknown file type");
			}
		}
	}
	catch (const fs::filesystem_error& e) {
		cout << e.code().message() << endl;
	}
	string temp = os.str();
	strcpy_s(dirList, DIR_BUFSIZE, temp.c_str());
	delete[] tempList;

	//sendMsg("150 Openning data channel for directory listing of " + getCurPath() + CRLF);

	return strlen(dirList);
}


void ControlHandler::pasvInitProcess() {
	int retval{ 0 };
	retval = recv(controlSock, responseBuf, COM_BUFSIZE, 0);   // receive welcome message
	if (retval == SOCKET_ERROR) {
		errorHandle->err_display("recv()");
	}
	responseHandler(); // welcome message processing

	sendMsg("PWD");
	controlRecv();
	responseHandler();

	sendMsg("PASV");
	controlRecv();
	responseHandler();


	sendMsg("LIST");
	retval = recv(dataConnectSock, dirList, DIR_BUFSIZE, 0);
	if (retval == SOCKET_ERROR) errorHandle->err_display("recv()");
	dirList[retval] = '\0';

	

	//qcout::cout << QString::fromLocal8Bit(dirList) << endl;
	//emit printDir(QString::fromLocal8Bit(dirList));
	emit printDirCwd(QString::fromLocal8Bit(dirList));

	//cout << dirList << endl;
	//DisplayDir(dirList);

	controlRecv();
	responseHandler();

}

