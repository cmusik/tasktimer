#ifndef TASKWINDOW_H
#define TASKWINDOW_H

#include <QMainWindow>

#include "TaskModel.h"
#include "TaskFilter.h"
#include "TaskNotes.h"

#include "ui_tasktimer.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/scrnsaver.h>

class TaskWindow : public QMainWindow, private Ui::TaskWindow {
	Q_OBJECT

	public:
		TaskWindow(QWidget* = 0);
		QString status();

	public slots:
		void startTask();

	private slots:
		//void stopTask();
		void addTask();
		void removeTask();
		void doneTask();
		void checkIdle();
		void setPriority(QAction*);
		void selectionChange(const QModelIndex&);
		void startNewSession();
		void showNotes();

	private:
		void closeEvent(QCloseEvent *);
		void updateStartAction(const QModelIndex&);

		TaskModel *m_data;
		TaskFilter *m_filter;
		XScreenSaverInfo *m_screensaver;
		QTimer *m_idleTimer;
};

#endif
