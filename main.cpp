#include <QApplication>
#include "JobWindow.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	JobWindow job;

	return app.exec();
}

