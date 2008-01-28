#ifndef  TASKNOTES_H
#define  TASKNOTES_H

#include <QDialog>

#include "Task.h"
#include "ui_tasknotes.h"

class TaskNotes : public QDialog, public Ui::TaskNotes {
	Q_OBJECT

	public:
		TaskNotes(QWidget*, QString);
		QString noteText();

	private:
};

#endif

