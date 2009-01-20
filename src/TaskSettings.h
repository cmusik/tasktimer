#ifndef  TASKSETTINGS_INC
#define  TASKSETTINGS_INC

#include <QWidget>
#include <QStringList>
#include <QListWidgetItem>
#include "ui_tasksettings.h"

class TaskSettings : public QDialog, private Ui::TaskSettings {
	Q_OBJECT

	public:
		TaskSettings(QWidget* = 0);
		void addGroups(QStringList);
		QStringList getGroups();

	private slots:
		void addGroup();
		void removeGroup();
		void moveGroupUp();
		void moveGroupDown();
};

#endif

