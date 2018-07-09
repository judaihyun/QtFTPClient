#include "FtpGui.h"

FtpGui::FtpGui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowTitle("Client");
	qRegisterMetaType<logLevel>("logLevel");

	initLocalList();
	initToDownList();

	
	ui.hostEdit->setText("127.0.0.1");
	//ui.hostEdit->setText("192.168.219.145");
	ui.portEdit->setText("210");


	connect(&widgetUi, SIGNAL(setLogLevel(logLevel)), this, SLOT(setLogLevel(logLevel)));

	connect(ui.actSetting, SIGNAL(triggered()), this, SLOT(popupSetting()));
	connect(ui.conBtn, SIGNAL(clicked()), this, SLOT(pasvConSlot()));  /*  connect to passive mode */

	connect(&workerThread, &QThread::finished, []() {std::cout << "workerThread finished\n"; });
	connect(&workerThread, &QThread::started, []() {std::cout << "workerThread started\n"; });

	connect(ui.actionRefresh, &QAction::triggered, this, &FtpGui::actRefresh);
	connect(ui.actionDisconnect, &QAction::triggered, this, &FtpGui::actDiscon);

	connect(ui.remoteTree, SIGNAL(clicked(const QModelIndex&)), this, SLOT(clickRow(const QModelIndex&)));
	
}

void FtpGui::initLocalList() {
	QFileSystemModel *model = new QFileSystemModel(this);
	model->setFilter(QDir::NoDotAndDotDot |
		QDir::AllDirs | QDir::Files);
	model->setRootPath(localPath);
	
	QHeaderView *localHeader = ui.localTree->header();
	ui.localTree->setHeader(localHeader);
	ui.localTree->setModel(model);

	localHeader->resizeSections(QHeaderView::Interactive);
	localHeader->resizeSection(0, 200);
	localHeader->resizeSection(1, 80);
	localHeader->resizeSection(2, 50);

	QModelIndex idx = model->index(localPath);
	ui.localTree->setRootIndex(idx);
	ui.localTree->show();

}


void FtpGui::initRemoteDir() {
	remoteModel = new QStandardItemModel(this);
	remoteModel->setSortRole(Qt::DecorationRole);
	remoteModel->setColumnCount(4);

	QString headerNames = "Name;Size;Type;Date Modified;Path";
	QStringList remoteHeaderName = headerNames.split(";");
	QHeaderView *remoteHeader = ui.remoteTree->header();
	remoteModel->setHorizontalHeaderLabels(remoteHeaderName);
	ui.remoteTree->setModel(remoteModel);
	remoteHeader->resizeSections(QHeaderView::Interactive);
	remoteHeader->resizeSection(0, 200);
	remoteHeader->resizeSection(1, 80);
	remoteHeader->resizeSection(2, 50);

	QList<QStandardItem*> item;
	item.push_front(new QStandardItem("")); //date
	item.push_front(new QStandardItem("<DIR>"));  //type
	item.push_front(new QStandardItem(""));  //  size
	item.push_front(new QStandardItem(style()->standardIcon(QStyle::SP_DirIcon), "/"));
	
	rootItem = new QStandardItem("/");
	item.push_back(rootItem);  //  Path
	remoteModel->appendRow(item);
	selectedIndex = rootItem->index();

	ui.remoteTree->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.remoteTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.remoteTree->sortByColumn(1, Qt::AscendingOrder);
	ui.remoteTree->setModel(remoteModel);
	ui.remoteTree->hideColumn(PATH_COL);

	ui.remoteTree->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(ui.remoteTree, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onCustomContextMenuRequested(const QPoint&)));
	
}


void FtpGui::initToDownList() {
	downlistModel = new QStandardItemModel(this);
	downlistModel->setColumnCount(3);

	QString headerNames = QString::fromLocal8Bit("서버/로컬 파일;방향;리모트 파일;크기");
	QStringList downHeaderName = headerNames.split(";");
	QHeaderView *downHeader = ui.downlistTree->header();

	downlistModel->setHorizontalHeaderLabels(downHeaderName);
	ui.downlistTree->setModel(downlistModel);
	ui.downlistTree->setEditTriggers(QAbstractItemView::NoEditTriggers);

	downHeader->resizeSections(QHeaderView::Interactive);
	downHeader->resizeSection(0, 400);
	downHeader->resizeSection(1, 80);
	downHeader->resizeSection(2, 400);


}

void FtpGui::recurDirList/*SLOT*/(const QString& dir, const QString pwd) { /*  to download list  */
	ftpLog(LOG_TRACE, "--- recurDirList() ---");
	int itemCount{ 0 };
	QStringList items = dir.split("\r\n");
	itemCount = dir.split("\r\n").length() - 1;
	qDebug() << pwd;
	for (int i = 0; i < itemCount; ++i) {
		QString fileName{ "" };
		QList<QStandardItem*> toDownFiles;
		QStringList row = items[i].split(" ", QString::SkipEmptyParts);
		extractFileName(row, fileName);

		if (row[TYPE_COL] == "<DIR>") {
			remoteCurPath += fileName.replace(" ","") + "/";
			emit sendCommand("CWDLIST " + remoteCurPath);
	
		}
		else {
			toDownFiles.push_back(new QStandardItem(localPath + pwd + fileName));  //  local
			toDownFiles.push_back(new QStandardItem("<----"));  //  direction 
			toDownFiles.push_back(new QStandardItem(remotePath + pwd + fileName));  //  remote
			toDownFiles.push_back(new QStandardItem(sizeFormat(row[2].toLongLong())));  // size 
			toDownFiles.push_back(new QStandardItem(pwd));  // path 
			downlistModel->appendRow(toDownFiles);
		}
		
	}
	recurDownload();
}

void FtpGui::recurDownload() {
	
	for (int i = 0; i < downlistModel->rowCount(); ++i) {
		QModelIndex index = downlistModel->index(i, 2);
		QVariant fileName = index.data(Qt::DisplayRole);

		QModelIndex indexPath = downlistModel->index(i, 4);
		QVariant filePath = indexPath.data(Qt::DisplayRole);
	
		emit sendCommand("CWD " + filePath.toString());
		emit sendCommand("RETR " + fileName.toString());
	
	}
}

void FtpGui::downloadClickedSlot/*SLOT*/(){
	ftpLog(LOG_TRACE, "--- downloadClickedSlot() ---");

	QModelIndex index = ui.remoteTree->indexAt(*point);
	QVariant typeTemp = index.siblingAtColumn(TYPE_COL).data(Qt::DisplayRole);
	if (typeTemp == "<DIR>") {
		QVariant dirPath = index.siblingAtColumn(NAME_COL).data(Qt::DisplayRole);
		remoteCurPath = "/" + dirPath.toString().replace(" ","") + "/";
		//qDebug() << "remoteCurPath : " << remoteCurPath;
		emit sendCommand("CWDLIST " + dirPath.toString());
		return;
	}


	QList<QStandardItem*> toDownFiles;
	QStringList qsTemp;
	for (int i = 0; i < 5; ++i) {
		qsTemp.append(index.siblingAtColumn(i).data(Qt::DisplayRole).toString());
	}
	toDownFiles.push_back(new QStandardItem(localPath + qsTemp[PATH_COL]));
	toDownFiles.push_back(new QStandardItem("<---"));
	toDownFiles.push_back(new QStandardItem(remotePath + qsTemp[PATH_COL]));
	toDownFiles.push_back(new QStandardItem(qsTemp[SIZE_COL]));
	downlistModel->appendRow(toDownFiles);
	
	emit sendCommand("RETR " + qsTemp[PATH_COL]);
}

void FtpGui::downloadCompleted/*SLOT*/() {
	downlistModel->takeRow(0);
}

void FtpGui::clickRow/*SLOT*/(const QModelIndex& index) {
	if (!index.isValid()) {
		qDebug() << "clickRow is not vaild()\n";
		return;
	}
	if (rootItem->index() == index.siblingAtColumn(PATH_COL)) {
		return;
	}
	
	// list에 index를 담아서 눌렀던 것들을 저장하고, for문으로 index를 각각 비교하여 꺼내여 removeRows해야할듯
	
	if (selectedIndex == index.siblingAtColumn(PATH_COL)) {
		int count = tempParent->child(index.row())->rowCount();
		remoteModel->removeRows(0, count, index.siblingAtColumn(0));
	}

	selectedIndex = index.siblingAtColumn(PATH_COL);
	

	QVariant value = selectedIndex.data(Qt::DisplayRole);
	colPath = value.toString();

	qDebug() << "clickRow : " << colPath;
	if (!colPath.isEmpty() && colPath != "" && index.siblingAtColumn(2).data(Qt::DisplayRole) == "<DIR>") {
		emit sendCommand("CWD " + colPath);
		ui.remoteTree->setExpanded(selectedIndex, true);
	}
}

void FtpGui::printDirCwd/*SLOT*/(const QString& dir) {
	ui.remoteTree->setExpanded(selectedIndex, true);
	int itemCount{ 0 };
	QStringList items = dir.split("\r\n");
	itemCount = dir.split("\r\n").length() - 1;

	tempParent = static_cast<QStandardItem*>(selectedIndex.internalPointer());
	
	QVariant indexData = selectedIndex.data(Qt::DisplayRole);
	QString parentPath = indexData.toString().left(indexData.toString().length() - 1);
	
	for (int i = 0; i < itemCount; ++i) {
		QString fileName{ "" };
		QList<QStandardItem*> item;
		QStringList row = items[i].split(" ", QString::SkipEmptyParts);
		extractFileName(row, fileName);

		item.push_front(new QStandardItem(row[YEAR] + " " + row[TIME])); // Date
		if (row[TYPE_COL] == "<DIR>") {
			item.push_front(new QStandardItem(row[TYPE_COL]));  //Type
			item.push_front(new QStandardItem(" ")); // Size
			item.push_front(new QStandardItem(style()->standardIcon(QStyle::SP_DirIcon), fileName));
		}
		else {
			item.push_front(new QStandardItem(" "));  //  Type
			item.push_front(new QStandardItem(sizeFormat(row[2].toLongLong())));  // Size 
			item.push_front(new QStandardItem(style()->standardIcon(QStyle::SP_FileIcon), fileName));
		}
		item.push_back(new QStandardItem(parentPath + "/" + fileName));  //  Path
	
		tempParent->child(selectedIndex.row())->appendRow(item);
	}

}

void FtpGui::actRefresh/*SLOT*/() {
	if (colPath.isEmpty()) {
		delete remoteModel;
		initRemoteDir();
		emit sendCommand("LIST ");
	}
	else {
		int count = tempParent->child(selectedIndex.row())->rowCount();
		remoteModel->removeRows(0, count, selectedIndex.siblingAtColumn(0));
		emit sendCommand("CWD " + colPath);
	}
}

void FtpGui::pasvConSlot()/*SLOT*/ {
	ftpLog(LOG_DEBUG, "enter pasvConSlot() ");
	
	initRemoteDir();

	passToThread arg;
	arg.serverIP = ui.hostEdit->text().toStdString();
	//arg.controlPort = ui.portEdit->text().toInt();
	/*
	arg.serverIP = "127.0.0.1";
	*/
	arg.controlPort = 210;


	

	FtpClient client(&arg, this);
	client.setRemotePath("f:"); //remote
	client.setLocalPath("e:"); //local
	client.moveToThread(&workerThread);
	connect(&workerThread, SIGNAL(started()), &client, SLOT(starter()));
	workerThread.start();

	ui.actionRefresh->setEnabled(true);
	ui.actionCancel->setEnabled(true);
	ui.actionDisconnect->setEnabled(true);
	ui.conBtn->setEnabled(false);

	
	int retcode = loop.exec();
	cout << "main loop exit()" << retcode << endl;
	

}
void FtpGui::connectFailed() {
	cout << "connectedfailed\n";
	loop.exit();
	workerThread.quit();
	
	delete remoteModel;
	ui.actionCancel->setEnabled(false);
	ui.actionDisconnect->setEnabled(false);
	ui.actionRefresh->setEnabled(false);
	ui.conBtn->setEnabled(true);
	
	ftpLog(LOG_INFO, "Disconnected");
	
}
void FtpGui::actDiscon/*SLOT*/() {
	ftpLog(LOG_DEBUG, "actionDisconnectButton clicked()");
	colPath = nullptr;
	emit exitCommand();
	loop.exit();
	workerThread.quit();

	ui.actionCancel->setEnabled(false);
	ui.actionDisconnect->setEnabled(false);
	ui.actionRefresh->setEnabled(false);
	ui.conBtn->setEnabled(true);

	ftpLog(LOG_INFO, "Disconnected");

}

void FtpGui::actCancel() {

}

void FtpGui::displayLog(const QString& input, QColor color) {
	ui.textEdit->setTextColor(color);
	ui.textEdit->append(input.toUtf8());
}
void FtpGui::ftpLog(logLevel level, const QString& qbuf) {
	QString temp{ "" };
	
	if (lev >= level) {
		switch (level) {
		case LOG_TRACE: {   // 6
			temp = QString("TRACE [ %1 ] %2").arg(QString::number(LOG_NUMBER++)).arg(qbuf);
			displayLog(temp, QColor(0,0,0));
			break;
		}
		case LOG_DEBUG: {   // 5
			temp = QString("DEBUG [ %1 ] %2").arg(QString::number(LOG_NUMBER++)).arg(qbuf);
			displayLog(temp, QColor(0,0,0));
			break;
		}
		case LOG_INFO: {  // 4
			temp = QString("INFO [ %1 ] %2").arg(QString::number(LOG_NUMBER++)).arg(qbuf);
			displayLog(temp, QColor(0,0,255));
			break;
		}
		case LOG_WARN: {  // 3
			temp = QString("WARN [ %1 ] %2").arg(QString::number(LOG_NUMBER++)).arg(qbuf);
			displayLog(temp, QColor(150,0,0));
			break;
		}
		case LOG_ERROR: {  // 2
			temp = QString("ERROR [ %1 ] %2").arg(QString::number(LOG_NUMBER++)).arg(qbuf);
			displayLog(temp, QColor(255,0,0));
			break;
		}
		case LOG_FETAL: {  // 1
			temp = QString("FETAL [ %1 ] %2").arg(QString::number(LOG_NUMBER++)).arg(qbuf);
			displayLog(temp, QColor(255,0,0));
			break;
		}
		}
	}
}

void FtpGui::onCustomContextMenuRequested/*SLOT*/(const QPoint& p) {

	point = &p;

	QMenu contextMenu;
	uninstallAction = new QAction("DownLoad");
	contextMenu.addAction(uninstallAction);
	connect(uninstallAction, SIGNAL(triggered()), this, SLOT(downloadClickedSlot()));
	contextMenu.exec(ui.remoteTree->mapToGlobal(*point));

	delete uninstallAction;
}

void FtpGui::onCustomContextMenuRequestedDownTree/*SLOT*/(const QPoint& p) {

	point = &p;

	QMenu contextMenu;
	uninstallAction = new QAction("delete item");
	contextMenu.addAction(uninstallAction);

//	connect(uninstallAction, SIGNAL(triggered()), this, SLOT(downloadClickedSlot()));

	contextMenu.exec(ui.downlistTree->mapToGlobal(*point));

	delete uninstallAction;
}