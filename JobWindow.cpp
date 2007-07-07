#include <QDebug>
#include <QModelIndex>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QCloseEvent>
#include "JobWindow.h"
#include "JobEdit.h"

JobWindow::JobWindow(QWidget *parent) : QMainWindow(parent) {
	setupUi(this);
	jobData = new JobModel();
	jobData->load();
	jobTable->setModel(jobData);
	JobEdit *delegeate = new JobEdit(this);
	jobTable->setItemDelegate(delegeate);

	connect(actionStart, SIGNAL(activated()), this, SLOT(startJob()));
	connect(actionStop, SIGNAL(activated()), this, SLOT(stopJob()));
	connect(actionNew, SIGNAL(activated()), this, SLOT(addJob()));
	connect(actionRemove, SIGNAL(activated()), this, SLOT(removeJob()));
	connect(actionSave, SIGNAL(activated()), jobData, SLOT(save()));

	jobTable->setSelectionModel(new QItemSelectionModel(jobData));
	jobTable->resizeColumnToContents(1);
	jobTable->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);

	show();
}

void JobWindow::startJob() {
	jobData->start(jobTable->currentIndex());
}

void JobWindow::stopJob() {
	jobData->stop(jobTable->currentIndex());
}

void JobWindow::addJob() {
	QModelIndexList selected = jobTable->selectionModel()->selectedRows();
	int pos = 0;
	if (selected.count())
		pos = selected.at(0).row();

	jobData->insertRows(pos, 1);
	jobTable->update();
}

void JobWindow::removeJob() {
	QModelIndexList selected = jobTable->selectionModel()->selectedRows();
	int pos = 0;
	if (selected.count())
		pos = selected.at(0).row();

	jobData->removeRows(pos, 1);
	jobTable->update();
}

void JobWindow::closeEvent(QCloseEvent *event) {
	jobData->save();
	event->accept();
}
