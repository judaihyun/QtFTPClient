#include "FtpGui.h"
#include <QtWidgets/QApplication>
#include <Windows.h>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FtpGui w;
	w.show();
	return a.exec();
}
