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
		void groupAdd(QStringList);
		QStringList groupGet();

	private slots:
		void groupAdd();
		void groupRemove();
		void groupMoveUp();
		void groupMoveDown();
		void groupRowChanged(int);
		void groupLineChanged(QString);
};

#endif

