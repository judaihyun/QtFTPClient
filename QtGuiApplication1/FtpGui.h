#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_QtGuiApplication1.h"
#include "FtpSettingWidget.h"
#include "WinSockHeader.h"
#include "FtpClient.h"
#include "Utils.h"
#include "ControlHandler.h"
#include <qaction.h>
#include <qdebug.h>
#include <qfilesystemmodel.h>
#include <qstandarditemmodel.h>
#include <qstringlistmodel.h>
#include "ErrorHandler.h"
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

class FtpGui : public QMainWindow
{
	Q_OBJECT

private:
	unsigned long int LOG_NUMBER{ 0 };
	QThread workerThread;
	QString localPath{ "e:" };
	QString remotePath{ "f:" };
	QString remoteCurPath{ "" };
	QEventLoop loop;
	QModelIndex selectedIndex;
	QStandardItemModel *remoteModel = nullptr;
	QStandardItemModel *downlistModel = nullptr;
	QStandardItem *rootItem = nullptr;
	QStandardItem *tempParent = nullptr;
	QString colPath;
	QAction *uninstallAction = nullptr;
	const QPoint *point;
	ErrorHandler *errorHandler = nullptr;
protected:
	Ui::QtGuiApplication1Class ui;
	FtpSettingWidget widgetUi;
	logLevel lev;
public:
	FtpGui(QWidget *parent = Q_NULLPTR);
	void displayLog(const QString& input, QColor color);
	void initRemoteDir();
	void initToDownList();
	void initLocalList();
	
	~FtpGui() {
		delete downlistModel;
		delete rootItem;
		delete tempParent;
		delete uninstallAction;
		delete remoteModel;
	}

	void recurDownload();
signals:
	void sendCommand(const QString&);
	void exitCommand();
	void customContextMenuRequestedDownTree(const QPoint&);

public slots:
	void setLocalPath(const QString path) { localPath = path; }
	void setLogLevel(logLevel l) { lev = l; }
	void popupSetting() { widgetUi.show(); }
	void connectFailed();
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
	void recurDirList(const QString& dir, const QString);
	void downloadCompleted();
	void nextToDownload() { qDebug() << "recurLoop.exit()"; /*recurLoop.exit();*/ };
};


