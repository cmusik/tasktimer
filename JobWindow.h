#include <QMainWindow>
#include "JobModel.h"
#include "ui_jobtimer.h"

class JobWindow : public QMainWindow, private Ui::JobWindow {
	Q_OBJECT

	public:
		JobWindow(QWidget* = 0);

	private slots:
		void startJob();
		void stopJob();

	private:
		JobModel *jobData;
};
