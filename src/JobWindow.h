#ifndef JOBWINDOW_H
#define JOBWINDOW_H

#include <QMainWindow>
#include "JobModel.h"
#include "JobFilter.h"

#include "ui_jobtimer.h"

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

	private:
		void closeEvent(QCloseEvent *);

		JobModel *m_data;
		JobFilter *m_filter;
};

#endif
