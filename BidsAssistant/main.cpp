#include "BidsAssistant.h"
#include <QtWidgets/QApplication>
#include <QStyleFactory>
#include <QTextCodec>
#include "GlobalDefine.h"

int main(int argc, char *argv[])
{
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForLocale(codec);
	QApplication a(argc, argv);
	QApplication::setStyle(QStyleFactory::create("fusion"));
	BidsAssistant w;
	w.setFixedSize(w.width(), w.height());
	w.setWindowTitle(QString(APP_TITLE) + APP_VERSION);
	w.setWindowIcon(QIcon(":/Resources/icon1.png"));
	//w.show();
	return a.exec();
}
