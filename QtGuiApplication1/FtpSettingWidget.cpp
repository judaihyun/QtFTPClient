#include "FtpSettingWidget.h"

FtpSettingWidget::FtpSettingWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowTitle(QString::fromLocal8Bit("¼³Á¤"));

	ui.logLevelBox->addItem("NONE");
	ui.logLevelBox->addItem("FETAL");
	ui.logLevelBox->addItem("ERROR");
	ui.logLevelBox->addItem("WARN");
	ui.logLevelBox->addItem("INFO");
	ui.logLevelBox->addItem("DEBUG");
	ui.logLevelBox->addItem("TRACE");

	ui.modeBox->addItem("Passive");
	ui.modeBox->addItem("Active");

	ui.pathEdit->setText("e:");

	connect(ui.closeBtn, SIGNAL(clicked()), this, SLOT(closeBtn()));
	connect(ui.okBtn, SIGNAL(clicked()), this, SLOT(okBtn()));

}
