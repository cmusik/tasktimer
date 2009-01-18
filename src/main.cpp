#include <QApplication>
#include <QDBusConnection>

#include "TaskWindow.h"
#include "TaskDBus.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	TaskWindow taskWindow;

	new DBusAdaptor(&app, &taskWindow);
	QDBusConnection dbuscon = QDBusConnection::connectToBus(QDBusConnection::SessionBus, "tasktimer");
	dbuscon.registerService("de.senfdax.tasktimer");
	dbuscon.registerObject("/TaskTimer", &app);

	return app.exec();
}

