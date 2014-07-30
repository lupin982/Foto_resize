#include "foto_resize.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Foto_resize w;
	w.show();
	return a.exec();
}
