#include "QtGuiApplication1.h"
#include <QtWidgets/QApplication>
#include <Windows.h>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtGuiMain w;
	w.show();
	return a.exec();
}
