#ifndef JOBWINDOW_H
#define JOBWINDOW_H

#include <QMainWindow>

#include "TaskModel.h"
#include "TaskFilter.h"

#include "ui_jobtimer.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/scrnsaver.h>

class TaskWindow : public QMainWindow, private Ui::TaskWindow {
	Q_OBJECT

	public:
		TaskWindow(QWidget* = 0);

	private slots:
		void startTask();
		void stopTask();
		void addTask();
		void removeTask();
		void doneTask();
		void checkIdle();
		void setPriority(QAction*);
		void selectionChange(const QModelIndex&);

	private:
		void closeEvent(QCloseEvent *);

		TaskModel *m_data;
		TaskFilter *m_filter;
		XScreenSaverInfo *m_screensaver;
		QTimer *m_idleTimer;
};

#endif
