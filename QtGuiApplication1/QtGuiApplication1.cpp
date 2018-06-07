#include "QtGuiApplication1.h"

QtGuiMain::QtGuiMain(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	qRegisterMetaType<logLevel>("logLevel");
	ui.hostEdit->setText("127.0.0.1");
	ui.portEdit->setText("210");

	connect(ui.conBtn, SIGNAL(clicked()), this, SLOT(pasvConSlot()));


	//connect(&workerThread, &QThread::finished, this, &QtGuiMain::actDiscon);
	connect(&workerThread, &QThread::finished, []() {std::cout << "workerThread finished\n"; });
	connect(&workerThread, &QThread::started, []() {std::cout << "workerThread started\n"; });

	connect(ui.actionRefresh, &QAction::triggered, this, &QtGuiMain::actRefresh);
	connect(ui.actionDisconnect, &QAction::triggered, this, &QtGuiMain::actDiscon);
	//connect(ui.actionDisconnect, &QAction::triggered, &loop, SLOT(quit()));

}

void QtGuiMain::actRefresh()
{
	cout << "refresh()\n";
	emit sendCommand("LIST");
}

void QtGuiMain::pasvConSlot() {
	if (workerThread.isRunning()) {
		cout << "thread is running\n";
		actDiscon();
	}
	passToThread arg;
	//arg.serverIP = ui.hostEdit->text().toStdString();
	//arg.controlPort = ui.portEdit->text().toInt();
	arg.serverIP = "127.0.0.1";
	arg.controlPort = 210;
	
	//ui.textEdit->append(ui.hostEdit->text()+" "+ui.portEdit->text());

	cout << "enter pasvConSlot() \n";

	client = new FtpClient(&arg,this);
	client->setPath("f:");

	connect(client, SIGNAL(printLog(logLevel, const QString&)), this, SLOT(ftpLog(logLevel, QString)));
	connect(&workerThread, SIGNAL(started()), client, SLOT(starter()));

	client->moveToThread(&workerThread);
	workerThread.start();
	

	ui.actionRefresh->setEnabled(true);
	ui.actionCancel->setEnabled(true);
	ui.actionDisconnect->setEnabled(true);


	loop.exec();
	delete client;
	cout << "main loop exit()" << endl;
}

void QtGuiMain::actDiscon() {
	ftpLog(LOG_INFO, "Disconnected");
	loop.exit();
	workerThread.quit();

	ui.actionCancel->setEnabled(false);
	ui.actionDisconnect->setEnabled(false);
	ui.actionRefresh->setEnabled(false);


	if (workerThread.isRunning()) {
		cout << "thread is running\n";
	}
	else {
		cout << "thread finsihed\n";
	}
}

void QtGuiMain::actCancel() {

}

void QtGuiMain::ftpLog(logLevel level, const QString& qbuf) {
	QString temp;
	lev = logLevel::LOG_TRACE;
	if (lev >= level) {
		switch (level) {
		case LOG_TRACE: {   // 6
			temp = QString("TRACE [ %1 ] %2").arg(QString::number(LOG_NUMBER++)).arg(qbuf);
			displayLog(temp);
			break;
		}
		case LOG_DEBUG: {   // 5
			temp = QString("DEBUG [ %1 ] %2").arg(QString::number(LOG_NUMBER++)).arg(qbuf);
			displayLog(temp);
			break;
		}
		case LOG_INFO: {  // 4
			temp = QString("INFO [ %1 ] %2").arg(QString::number(LOG_NUMBER++)).arg(qbuf);
			displayLog(temp);
			break;
		}
		case LOG_WARN: {  // 3
			temp = QString("WARN [ %1 ] %2").arg(QString::number(LOG_NUMBER++)).arg(qbuf);
			displayLog(temp);
			break;
		}
		case LOG_ERROR: {  // 2
			temp = QString("ERROR [ %1 ] %2").arg(QString::number(LOG_NUMBER++)).arg(qbuf);
			displayLog(temp);
			break;
		}
		case LOG_FETAL: {  // 1
			temp = QString("FETAL [ %1 ] %2").arg(QString::number(LOG_NUMBER++)).arg(qbuf);
			displayLog(temp);
			break;
		}
		}
	}
}
