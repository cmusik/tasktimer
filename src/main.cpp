#include <QApplication>
#include "TaskWindow.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	TaskWindow job;

	return app.exec();
}

