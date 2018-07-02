#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_QtGuiApplication1.h"
#include "WinSockHeader.h"
#include "FtpClient.h"
#include "Utils.h"
#include "ControlHandler.h"
#include <qaction.h>
#include <qdebug.h>
#include <qfilesystemmodel.h>
#include <qstandarditemmodel.h>
#include <qstringlistmodel.h>
#define YEAR 0
#define TIME 1
#define NAME_COL 0
#define SIZE_COL 1
#define TYPE_COL 2
#define DATE_COL 3
#define PATH_COL 4
namespace Ui {
	class QtGuiMain;
}

class QtGuiMain : public QMainWindow
{
	Q_OBJECT

private:
	unsigned long int LOG_NUMBER{ 0 };
	QThread workerThread;
	QString localPath{ "e:" };
	QString remotePath{ "f:" };
	QEventLoop loop;
	QModelIndex selectedIndex;
	QStandardItemModel *remoteModel;
	QStandardItemModel *downlistModel;
	QStandardItem *rootItem;
	QStandardItem *tempParent;
	QString colPath;
	QAction* uninstallAction;
	const QPoint *point;
protected:
	Ui::QtGuiApplication1Class ui;
	logLevel lev;
public:
	QtGuiMain(QWidget *parent = Q_NULLPTR);
	void displayLog(const QString& input) { ui.textEdit->append(input); }
	void initRemoteDir();
	void initToDownList();
	void initLocalList();
	
	~QtGuiMain() {
		delete downlistModel;
		delete rootItem;
		delete tempParent;
		delete uninstallAction;
		delete remoteModel;
	}

signals:
	void sendCommand(const QString&);
	void exitCommand();
	void customContextMenuRequestedDownTree(const QPoint&);

public slots:
	void ftpLog(logLevel, const QString& input);
	void actRefresh();
	void printDirCwd(const QString& dir);
	void pasvConSlot();
	void actCancel();
	void actDiscon();
	void clickRow(const QModelIndex&);
	void onCustomContextMenuRequested(const QPoint& point);
	void onCustomContextMenuRequestedDownTree(const QPoint& point);
	void downloadClickedSlot();
	void recurDirList(const QString& dir);
};


