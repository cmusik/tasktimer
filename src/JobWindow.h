#ifndef JOBWINDOW_H
#define JOBWINDOW_H

#include <QMainWindow>

#include "JobModel.h"
#include "JobFilter.h"

#include "ui_jobtimer.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/scrnsaver.h>

class JobWindow : public QMainWindow, private Ui::JobWindow {
	Q_OBJECT

	public:
		JobWindow(QWidget* = 0);

	private slots:
		void startJob();
		void stopJob();
		void addJob();
		void removeJob();
		void doneJob();
		void checkIdle();

	private:
		void closeEvent(QCloseEvent *);

		JobModel *m_data;
		JobFilter *m_filter;
		XScreenSaverInfo *m_screensaver;
		QTimer *m_idleTimer;
};

#endif
