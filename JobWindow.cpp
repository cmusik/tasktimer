#include <QDebug>
#include <QModelIndex>
#include "JobWindow.h"
#include "JobEdit.h"

JobWindow::JobWindow(QWidget *parent) : QMainWindow(parent) {
	setupUi(this);
	jobData = new JobModel();
	jobTable->setModel(jobData);
	JobEdit *delegeate = new JobEdit(this);
	jobTable->setItemDelegate(delegeate);

	connect(actionStart, SIGNAL(activated()), this, SLOT(startJob()));
	connect(actionStop, SIGNAL(activated()), this, SLOT(stopJob()));
	connect(actionNew, SIGNAL(activated()), this, SLOT(addJob()));

	show();
}

void JobWindow::startJob() {
	jobData->start(jobTable->currentIndex());
}

void JobWindow::stopJob() {
	jobData->stop(jobTable->currentIndex());
}

void JobWindow::addJob() {
	jobData->insertRows(0, 3);
	jobTable->update();
}
