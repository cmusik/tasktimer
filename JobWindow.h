#include <QMainWindow>
#include <QTimer>
#include "JobModel.h"
#include "ui_jobtimer.h"

class JobWindow : public QMainWindow, private Ui::JobWindow {
	Q_OBJECT

	public:
		JobWindow(QWidget* = 0);

	private slots:
		void startJob();
		void stopJob();
		void updateTable();

	private:
		JobModel *jobData;
		QTimer *timer;
};
