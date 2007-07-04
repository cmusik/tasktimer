#include <QDebug>
#include "JobWindow.h"

JobWindow::JobWindow(QWidget *parent) : QMainWindow(parent) {
	setupUi(this);
	jobData = new JobModel();
	jobTable->setModel(jobData);

	connect(actionStart, SIGNAL(activated()), this, SLOT(startJob()));
	connect(actionStop, SIGNAL(activated()), this, SLOT(stopJob()));
	timer = new QTimer();
	timer->setInterval(100);
	timer->start();

	connect(timer, SIGNAL(timeout()), this, SLOT(updateTable()));

	show();
}

void JobWindow::startJob() {
	jobData->start(jobTable->currentIndex());
}

void JobWindow::stopJob() {
	jobData->stop(jobTable->currentIndex());
}

void JobWindow::updateTable() {
	qDebug() << "update";
	jobTable->update();
}
