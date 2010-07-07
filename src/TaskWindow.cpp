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
#include "TaskSettings.h"

TaskWindow::TaskWindow(QWidget *parent) : QMainWindow(parent) {
	setupUi(this);
	m_data = new TaskModel(this);
	m_data->load();

	m_filter = new TaskFilter(this);
	m_filter->setSourceModel(m_data);
	taskTable->setModel(m_filter);

	m_screensaver = XScreenSaverAllocInfo();

	m_idleTimer= new QTimer(this);
	m_idleTimer->setInterval(1000);

	connect(m_idleTimer, SIGNAL(timeout()), this, SLOT(checkIdle()));

	connect(actionStart, SIGNAL(activated()), this, SLOT(startTask()));
	//connect(actionStop, SIGNAL(activated()), this, SLOT(stopTask()));

	connect(actionStopAll, SIGNAL(activated()), m_data, SLOT(stopAll()));
	connect(actionStopAll, SIGNAL(activated()), m_idleTimer, SLOT(stop()));

	connect(actionNew, SIGNAL(activated()), this, SLOT(addTask()));
	connect(actionRemove, SIGNAL(activated()), this, SLOT(removeTask()));
	connect(actionDone, SIGNAL(activated()), this, SLOT(doneTask()));
	connect(actionHideDone, SIGNAL(triggered(bool)), m_filter, SLOT(filterDone(bool)));
	connect(actionNewSession, SIGNAL(activated()), this, SLOT(startNewSession()));
	connect(actionShowNotes, SIGNAL(activated()), this, SLOT(showNotes()));
	connect(actionSettings, SIGNAL(activated()), this, SLOT(showSettings()));

	taskTable->setSelectionModel(new QItemSelectionModel(m_filter));
	connect(taskTable->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(selectionChange(const QModelIndex&)));
	taskTable->resizeColumnToContents(Counter);
	taskTable->resizeColumnToContents(Priority);
	taskTable->resizeColumnToContents(TotalTime);
	taskTable->resizeColumnToContents(SessionTime);
	taskTable->horizontalHeader()->setResizeMode(Counter, QHeaderView::Fixed);
	taskTable->horizontalHeader()->setResizeMode(Priority, QHeaderView::Fixed);
	taskTable->horizontalHeader()->setResizeMode(TotalTime, QHeaderView::Fixed);
	taskTable->horizontalHeader()->setResizeMode(SessionTime, QHeaderView::Fixed);
	taskTable->horizontalHeader()->setResizeMode(Group, QHeaderView::Fixed);
	taskTable->horizontalHeader()->setResizeMode(Name, QHeaderView::Stretch);
	taskTable->verticalHeader()->hide();


	QActionGroup *priority = new QActionGroup(this);
	priority->addAction(actionPrio1);
	priority->addAction(actionPrio2);
	priority->addAction(actionPrio3);

	actionPrio1->setData(1);
	actionPrio2->setData(2);
	actionPrio3->setData(3);

	taskTable->addAction(actionStart);
	QAction *sep = new QAction("Priority", this);
	sep->setSeparator(true);
	taskTable->addAction(sep);
	taskTable->addAction(actionPrio1);
	taskTable->addAction(actionPrio2);
	taskTable->addAction(actionPrio3);

	QSettings settings("TaskTimer", "tasktimer");
	m_filter->filterDone(settings.value("hide_done").toBool());
	actionHideDone->setChecked(settings.value("hide_done").toBool());
	Qt::ToolBarArea area = Qt::TopToolBarArea;
	if (settings.contains("toolbar_pos"))
		area = (Qt::ToolBarArea) settings.value("toolbar_pos").toInt();

	if (area)
		addToolBar(area, toolBar);

	m_delegeate = new TaskEdit(this);

	if (settings.contains("groups")) {
		QStringList l = settings.value("groups").toStringList();
		m_delegeate->setGroups(QStringList(l));
	}

	taskTable->setItemDelegate(m_delegeate);

	connect(priority, SIGNAL(triggered(QAction*)), this, SLOT(setPriority(QAction*)));

	show();
}

void TaskWindow::startTask() {
	QModelIndexList selected = taskTable->selectionModel()->selectedRows();
	if (selected.count() == 1) {
		const QModelIndex &index = m_filter->mapToSource(selected.at(0));

		m_data->start(index);
		if (m_data->hasActive()) {
			setWindowTitle(QString("TaskTimer %1").arg(m_data->status()));
			m_idleTimer->start();
		}
		else {
			setWindowTitle(QString("TaskTimer"));
			m_idleTimer->stop();
		}

		updateStartAction(index);
	}
}

/*
void TaskWindow::stopTask() {
	QModelIndexList selected = taskTable->selectionModel()->selectedRows();
	if (selected.count() == 1) {
		const QModelIndex &index = m_filter->mapToSource(selected.at(0));

		m_data->stop(index);
	}
}
*/

void TaskWindow::addTask() {
	m_data->insertRows(m_data->rowCount(QModelIndex())-1, 1);

	taskTable->resizeColumnToContents(Counter);
	taskTable->resizeColumnToContents(Priority);
	taskTable->resizeColumnToContents(TotalTime);
}

void TaskWindow::removeTask() {
	QMessageBox::StandardButton ret;
	ret = QMessageBox::question(this, tr("Removing Task..."),
			tr("Do you really want to remove this task?"),
			QMessageBox::Yes | QMessageBox::No);

	if (ret == QMessageBox::No)
		return;

	QModelIndexList selected = taskTable->selectionModel()->selectedRows();
	if (selected.count() == 1) {
		int pos = m_filter->mapToSource(selected.at(0)).row();
		m_data->removeRows(pos, 1);
	}
}

void TaskWindow::closeEvent(QCloseEvent *event) {
	if (m_data->hasActive()) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::question(this, tr("Quitting TaskTimer..."),
				tr("There are running tasks. Do you want to save and quit?"),
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
	settings.setValue("groups", m_delegeate->getGroups());

	event->accept();
}

void TaskWindow::doneTask() {
	QModelIndexList selected = taskTable->selectionModel()->selectedRows();
	if (selected.count() == 1) {
		const QModelIndex &index = m_filter->mapToSource(selected.at(0));

		m_data->setDone(m_data->isDone(index) ? false : true, index);
		m_filter->invalidate();
	}
}

void TaskWindow::checkIdle() {
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
	QModelIndexList selected = taskTable->selectionModel()->selectedRows();
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
		default:
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
	updateStartAction(i);
}

void TaskWindow::startNewSession() {
	QModelIndexList selected = taskTable->selectionModel()->selectedRows();
	if (selected.count() == 1) {
		const QModelIndex &index = m_filter->mapToSource(selected.at(0));

		m_data->startNewSession(index);
	}
}

void TaskWindow::updateStartAction(const QModelIndex& index) {
	if (!index.isValid())
		return;

	if (m_data->isActive(index)) {
		actionStart->setIcon(QIcon(QString(":/icons/media-playback-pause.png")));
		actionStart->setText("Stop");
	}
	else {
		actionStart->setIcon(QIcon(QString(":/icons/media-playback-start.png")));
		actionStart->setText("Start");
	}
}


void TaskWindow::showNotes() {
	QModelIndexList selected = taskTable->selectionModel()->selectedRows();
	if (selected.count() == 1) {
		const QModelIndex &index = m_filter->mapToSource(selected.at(0));

		Task *t = m_data->getTask(index);
		TaskNotes *tw = new TaskNotes(this, t->note());
		if (tw->exec()) {
			t->setNote(tw->noteText());
		}
	}
}

QString TaskWindow::status() {
	return m_data->status();
}

void TaskWindow::showSettings() {
	TaskSettings s;
	s.groupAdd(m_delegeate->getGroups());
	if (s.exec() == QDialog::Accepted) {
		m_delegeate->setGroups(s.groupGet());
	}
}
