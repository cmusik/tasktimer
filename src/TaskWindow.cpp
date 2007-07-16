#include <QDebug>
#include <QModelIndex>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QCloseEvent>
#include <QMessageBox>
#include <QX11Info>
#include <QTimer>
#include <QDateTime>
#include <QActionGroup>
#include <QSettings>

#include "TaskWindow.h"
#include "TaskEdit.h"

TaskWindow::TaskWindow(QWidget *parent) : QMainWindow(parent) {
	setupUi(this);
	m_data = new TaskModel(this);
	m_data->load();

	m_filter = new TaskFilter(this);
	m_filter->setSourceModel(m_data);
	jobTable->setModel(m_filter);

	TaskEdit *delegeate = new TaskEdit(this);
	jobTable->setItemDelegate(delegeate);

	m_screensaver = XScreenSaverAllocInfo();

	m_idleTimer= new QTimer(this);
	m_idleTimer->setInterval(1000);

	connect(m_idleTimer, SIGNAL(timeout()), this, SLOT(checkIdle()));

	connect(actionStart, SIGNAL(activated()), this, SLOT(startTask()));
	connect(actionStop, SIGNAL(activated()), this, SLOT(stopTask()));

	connect(actionStopAll, SIGNAL(activated()), m_data, SLOT(stopAll()));
	connect(actionStopAll, SIGNAL(activated()), m_idleTimer, SLOT(stop()));

	connect(actionNew, SIGNAL(activated()), this, SLOT(addTask()));
	connect(actionRemove, SIGNAL(activated()), this, SLOT(removeTask()));
	connect(actionDone, SIGNAL(activated()), this, SLOT(doneTask()));
	connect(actionHideDone, SIGNAL(triggered(bool)), m_filter, SLOT(filterDone(bool)));

	jobTable->setSelectionModel(new QItemSelectionModel(m_filter));
	connect(jobTable->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(selectionChange(const QModelIndex&)));
	jobTable->resizeColumnToContents(Counter);
	jobTable->resizeColumnToContents(Priority);
	jobTable->resizeColumnToContents(ColumnTime);
	jobTable->horizontalHeader()->setResizeMode(Counter, QHeaderView::Fixed);
	jobTable->horizontalHeader()->setResizeMode(Priority, QHeaderView::Fixed);
	jobTable->horizontalHeader()->setResizeMode(Name, QHeaderView::Stretch);
	jobTable->verticalHeader()->hide();


	QActionGroup *priority = new QActionGroup(this);
	priority->addAction(actionPrio0);
	priority->addAction(actionPrio1);
	priority->addAction(actionPrio2);
	priority->addAction(actionPrio3);

	actionPrio0->setData(0);
	actionPrio1->setData(1);
	actionPrio2->setData(2);
	actionPrio3->setData(3);

	//QMenu *stopMenu = new QMenu(this);
	//actionStopAll->setShortcut(Qt::Key_Escape);
	//stopMenu->addAction(actionStopAll);
	//actionStop->setMenu(stopMenu);

	QSettings settings("TaskTimer", "tasktimer");
	m_filter->filterDone(settings.value("hide_done").toBool());
	actionHideDone->setChecked(settings.value("hide_done").toBool());
	Qt::ToolBarArea area = Qt::TopToolBarArea;
	if (settings.contains("toolbar_pos"))
		area = (Qt::ToolBarArea) settings.value("toolbar_pos").toInt();

	if (area)
		addToolBar(area, toolBar);

	connect(priority, SIGNAL(triggered(QAction*)), this, SLOT(setPriority(QAction*)));

	show();
}

void TaskWindow::startTask() {
	QModelIndexList selected = jobTable->selectionModel()->selectedRows();
	if (selected.count() == 1) {
		const QModelIndex &index = m_filter->mapToSource(selected.at(0));

		m_data->start(index);
	}
}

void TaskWindow::stopTask() {
	QModelIndexList selected = jobTable->selectionModel()->selectedRows();
	if (selected.count() == 1) {
		const QModelIndex &index = m_filter->mapToSource(selected.at(0));

		m_data->stop(index);
	}
}

void TaskWindow::addTask() {
	QModelIndexList selected = jobTable->selectionModel()->selectedRows();
	int pos = -1;
	if (selected.count())
		pos = m_filter->mapToSource(selected.at(0)).row();

	m_data->insertRows(pos, 1);

	jobTable->resizeColumnToContents(Counter);
	jobTable->resizeColumnToContents(Priority);
	jobTable->resizeColumnToContents(ColumnTime);
}

void TaskWindow::removeTask() {
	QModelIndexList selected = jobTable->selectionModel()->selectedRows();
	if (selected.count() == 1) {
		int pos = m_filter->mapToSource(selected.at(0)).row();
		m_data->removeRows(pos, 1);
	}
}

void TaskWindow::closeEvent(QCloseEvent *event) {
	if (m_data->hasActive()) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::question(this, tr("Quit TaskTimer?"),
				tr("There are running jobs. Do you want to save and quit?"),
				QMessageBox::Yes | QMessageBox::No);

		if (ret != QMessageBox::Yes) {
			event->ignore();
			return;
		}
	}
	m_data->save();

	// settings get cleared with save()
	QSettings settings("TaskTimer", "tasktimer");
	settings.setValue("hide_done", actionHideDone->isChecked());
	settings.setValue("toolbar_pos", toolBarArea(toolBar));

	event->accept();
}

void TaskWindow::doneTask() {
	QModelIndexList selected = jobTable->selectionModel()->selectedRows();
	if (selected.count() == 1) {
		const QModelIndex &index = m_filter->mapToSource(selected.at(0));

		m_data->setDone(m_data->isDone(index) ? false : true, index);
		m_filter->invalidate();
	}
}

void TaskWindow::checkIdle() {
	if (!m_data->hasActive()) {
		m_idleTimer->stop();
		return;
	}
	XScreenSaverQueryInfo(QX11Info::display (), QX11Info::appRootWindow(), m_screensaver);

	int idleMinutes = (m_screensaver->idle/1000)/60;

	if (idleMinutes >= 3) {
		m_idleTimer->stop();

		QDateTime stoppedTime(QDateTime::currentDateTime().addSecs(-3*60));

		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("Idle detected"),
				tr("Idle since %1. Do you want to revert timing?").arg(stoppedTime.toString()),
				QMessageBox::Yes | QMessageBox::No);

		if (ret == QMessageBox::Yes) {
			uint eclapsedTime = QDateTime::currentDateTime().toTime_t() -stoppedTime.toTime_t();
			m_data->revertActive(eclapsedTime);
		}
		m_idleTimer->start();
	}
}

void TaskWindow::setPriority(QAction *action) {
	QModelIndexList selected = jobTable->selectionModel()->selectedRows();
	if (selected.count() == 1) {
		const QModelIndex &index = m_filter->mapToSource(selected.at(0));
		m_data->setPriority(action->data().toInt(), index);
	}
}

void TaskWindow::selectionChange(const QModelIndex &index) {
	if (!index.isValid())
		return;
	const QModelIndex &i = m_filter->mapToSource(index);

	switch (i.model()->data(i, 34).toInt()) {
		case 0:
			actionPrio0->setChecked(true);
			break;
		case 1:
			actionPrio1->setChecked(true);
			break;
		case 2:
			actionPrio2->setChecked(true);
			break;
		case 3:
			actionPrio3->setChecked(true);
			break;
	}
}
