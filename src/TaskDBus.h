#ifndef  TASKDBUS_INC
#define  TASKDBUS_INC

#include <QApplication>
#include <QDBusAbstractAdaptor>

#include "TaskWindow.h"

class DBusAdaptor : public QDBusAbstractAdaptor {
	Q_OBJECT
		Q_CLASSINFO("D-Bus Interface", "de.senfdax.tasktimer")

	public:
		DBusAdaptor(QApplication*, TaskWindow*);

	public slots:
		QString status();
		void startSelected();

	private:
		QApplication *app;
		TaskWindow *gui;
};

#endif

