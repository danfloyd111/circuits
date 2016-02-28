#include <QApplication>
#include <circuitsWindow.h>

int main(int argc, char** argv){
	QApplication app(argc, argv);
	CircuitsWindow cw;
	cw.show();
	return app.exec();
}