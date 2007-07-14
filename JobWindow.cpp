#include <QDebug>
#include <QModelIndex>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QCloseEvent>
#include <QMessageBox>
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
	connect(actionDone, SIGNAL(activated()), this, SLOT(doneJob()));

	jobTable->setSelectionModel(new QItemSelectionModel(jobData));
	jobTable->resizeColumnToContents(Counter);
	jobTable->resizeColumnToContents(Time);
	jobTable->horizontalHeader()->setResizeMode(Name, QHeaderView::Stretch);
	jobTable->verticalHeader()->hide();

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

	jobTable->resizeColumnToContents(Counter);
	jobTable->resizeColumnToContents(Time);
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
	if (jobData->hasActive()) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("Application"),
				tr("There are running jobs. Do you want to save and quit?"),
				QMessageBox::Yes | QMessageBox::No);

		if (ret != QMessageBox::Yes) {
			event->ignore();
			return;
		}
	}
	jobData->save();
	event->accept();
}

void JobWindow::doneJob() {
	jobData->setDone(jobData->isDone(jobTable->currentIndex()) ? false : true, jobTable->currentIndex());
}
