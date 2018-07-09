#pragma once

#include <QWidget>
#include <qdebug.h>
#include "WinSockHeader.h"
#include "ui_FtpSettingWidget.h"


class FtpSettingWidget : public QWidget
{
	Q_OBJECT

public:
	FtpSettingWidget(QWidget *parent = Q_NULLPTR);
	~FtpSettingWidget() {  };
signals:
	void setLogLevel(logLevel);
	void setLocalPath(const QString);
public slots:
	void closeBtn() { FtpSettingWidget::close(); }
	void okBtn() {
		emit setLogLevel((logLevel)ui.logLevelBox->currentIndex());
		emit setLocalPath(ui.pathEdit->text());
		closeBtn();
	}
private:
	Ui::FtpSettingWidget ui;
};
