#include <QApplication>
#include "TaskWindow.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	TaskWindow taskWindow;

	return app.exec();
}

