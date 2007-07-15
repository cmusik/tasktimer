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
	m_data = new JobModel();
	m_data->load();

	m_filter = new JobFilter(this);
	m_filter->setSourceModel(m_data);
	jobTable->setModel(m_filter);

	JobEdit *delegeate = new JobEdit(this);
	jobTable->setItemDelegate(delegeate);

	connect(actionStart, SIGNAL(activated()), this, SLOT(startJob()));
	connect(actionStop, SIGNAL(activated()), this, SLOT(stopJob()));
	connect(actionNew, SIGNAL(activated()), this, SLOT(addJob()));
	connect(actionRemove, SIGNAL(activated()), this, SLOT(removeJob()));
	connect(actionDone, SIGNAL(activated()), this, SLOT(doneJob()));

	connect(actionHideDone, SIGNAL(triggered(bool)), m_filter, SLOT(filterDone(bool)));

	jobTable->setSelectionModel(new QItemSelectionModel(m_filter));
	jobTable->resizeColumnToContents(Counter);
	jobTable->resizeColumnToContents(Time);
	jobTable->horizontalHeader()->setResizeMode(Name, QHeaderView::Stretch);
	jobTable->verticalHeader()->hide();

	show();
}

void JobWindow::startJob() {
	m_data->start(jobTable->currentIndex());
}

void JobWindow::stopJob() {
	m_data->stop(jobTable->currentIndex());
}

void JobWindow::addJob() {
	QModelIndexList selected = jobTable->selectionModel()->selectedRows();
	int pos = 0;
	if (selected.count())
		pos = m_filter->mapToSource(selected.at(0)).row();

	m_data->insertRows(pos, 1);

	jobTable->resizeColumnToContents(Counter);
	jobTable->resizeColumnToContents(Time);
}

void JobWindow::removeJob() {
	QModelIndexList selected = jobTable->selectionModel()->selectedRows();
	int pos = 0;
	if (selected.count())
		pos = m_filter->mapToSource(selected.at(0)).row();

	m_data->removeRows(pos, 1);
	jobTable->update();
}

void JobWindow::closeEvent(QCloseEvent *event) {
	if (m_data->hasActive()) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("Application"),
				tr("There are running jobs. Do you want to save and quit?"),
				QMessageBox::Yes | QMessageBox::No);

		if (ret != QMessageBox::Yes) {
			event->ignore();
			return;
		}
	}
	m_data->save();
	event->accept();
}

void JobWindow::doneJob() {
	m_data->setDone(m_data->isDone(jobTable->currentIndex()) ? false : true, jobTable->currentIndex());
}
