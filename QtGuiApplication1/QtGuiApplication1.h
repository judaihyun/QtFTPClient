#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_QtGuiApplication1.h"
#include "WinSockHeader.h"
#include "FtpClient.h"
#include "ControlHandler.h"
#include <qdebug.h>
#include <qaction.h>

namespace Ui {
	class QtGuiMain;
}

class QtGuiMain : public QMainWindow
{
	Q_OBJECT

private:
	unsigned long int LOG_NUMBER{ 0 };
	QThread workerThread;
	FtpClient *client;
	QEventLoop loop;
public:
	QtGuiMain(QWidget *parent = Q_NULLPTR);
	void displayLog(const QString& input) { ui.textEdit->append(input); }
	~QtGuiMain() {
		workerThread.quit();
		workerThread.exit();
	}

protected:
	Ui::QtGuiApplication1Class ui;
	logLevel lev;

signals:
	void sendCommand(const QString&);
public slots:
	void ftpLog(logLevel, const QString& input);
	//void commandProcessor(const QString&);
	void actRefresh();
	void pasvConSlot();
	void actCancel();
	void actDiscon();


};


