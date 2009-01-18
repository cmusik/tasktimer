#include "TaskDBus.h"

DBusAdaptor::DBusAdaptor(QApplication *a, TaskWindow *w) : QDBusAbstractAdaptor(a), app(a), gui(w) {
}

QString DBusAdaptor::status() {
	return gui->status();
}

void DBusAdaptor::startSelected() {
	gui->startTask();
}
