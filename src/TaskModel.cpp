#include <QVariant>
#include <QDebug>
#include <unistd.h>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QStringList>
#include <QSettings>
#include <QDateTime>
#include "TaskModel.h"
#include "Task.h"


TaskModel::TaskModel(QObject *parent) : QAbstractTableModel(parent) {
	tasks = new QList<Task*>();

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateData()));
	timer->start(1000);

	QTimer *savetimer = new QTimer(this);
	connect(savetimer, SIGNAL(timeout()), this, SLOT(save()));
	savetimer->start(6000);
}

TaskModel::~TaskModel() {
}

void TaskModel::updateData() {
	for(int i = 0; i < tasks->count(); ++i) {
		if (tasks->at(i)->isStarted()) {
			emit dataChanged(index(i, TotalTime), index(i, SessionTime));
		}
	}
}

int TaskModel::rowCount(const QModelIndex&) const {
	return tasks->count();
}

int TaskModel::columnCount(const QModelIndex&) const {
	return 6;
}

QVariant TaskModel::data(const QModelIndex& index, int role) const {
	if (role == Active)
		return QVariant(isActive(index));

	if (role == Done)
		return QVariant(isDone(index));

	if (role == 34)
		return priority(index);

	if (role != Qt::DisplayRole)
		return QVariant();

	if (!index.isValid())
		return QVariant();


	switch(index.column()) {
		case Counter:
			return index.row()+1;
		case Priority:
			return tasks->at(index.row())->priority();
		case Name:
			return tasks->at(index.row())->name();
		case TotalTime:
			return timeToString(tasks->at(index.row())->totalElapsed());
		case SessionTime:
			return timeToString(tasks->at(index.row())->sessionElapsed());
		case Group:
			return tasks->at(index.row())->group();
	}
	return QVariant();
}

QString TaskModel::timeToString(uint time) const {
	int days = 0;
	int hours = 0;
	int mins = 0;
	int secs = 0;

	if (time > 86400) {
		days = time/86400;
		time -= days*86400;
	}

	if (time > 3600) {
		hours = time/3600;
		time -= hours*3600;
	}

	if (time > 60) {
		mins = time/60;
		time -= mins*60;
	}

	secs = time;

	if (days)
		return QString("%1d %2:%3:%4").arg(days).arg(hours, 2, 10, QChar('0')).arg(mins, 2, 10, QChar('0')).arg(secs, 2, 10, QChar('0'));
	else
		return QString("%1:%2:%3").arg(hours, 2, 10, QChar('0')).arg(mins, 2, 10, QChar('0')).arg(secs, 2, 10, QChar('0'));
}

void TaskModel::start(const QModelIndex& index) {
	Task *t = tasks->at(index.row());

	if (t->isStarted()) {
		t->stop();
	}
	else {
		t->start();
	}

	t->setDone(false);
	emit dataChanged(this->index(0, 0), this->index(tasks->count()-1, 3));
}

QVariant TaskModel::headerData (int section, Qt::Orientation orientation, int role) const {
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Horizontal) {
		switch(section) {
			case Counter:
				return QVariant();
			case Priority:
				return QString("Priority");
			case Name:
				return QString("Name");
			case TotalTime:
				return QString("Total");
			case SessionTime:
				return QString("Session");
			case Group:
				return QString("Group");
		}
	}
	else {
		return section+1;
	}

	return QVariant();
}

Qt::ItemFlags TaskModel::flags(const QModelIndex& index) const {
	Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);

	if (index.isValid())
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
	else {
	qDebug() << index;
		return Qt::ItemIsDropEnabled | defaultFlags;
	}

}

bool TaskModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if (!index.isValid())
		return false;
	if (role != Qt::EditRole)
		return false;

	switch(index.column()) {
		case Name:
			tasks->at(index.row())->setName(value.toString());
			break;
		case TotalTime:
			tasks->at(index.row())->addTotalTime(value.toInt()*60);
			break;
		case SessionTime:
			tasks->at(index.row())->addSessionTime(value.toInt()*60);
			break;
		case Group:
			tasks->at(index.row())->setGroup(value.toString());
		default:
			return false;
	}
	emit dataChanged(index, index);
	return true;
}

bool TaskModel::insertRows(int pos, int count, const QModelIndex&) {
	beginInsertRows(QModelIndex(), pos+1, pos+count);

	for (int i = 0; i < count; ++i)
		tasks->insert(pos+1, new Task(QString(), this));

	endInsertRows();
	return true;
}

bool TaskModel::insertRow(int pos, const QModelIndex&) {
	return insertRows(pos, 1);
}

bool TaskModel::removeRows(int pos, int count, const QModelIndex&) {
	beginRemoveRows(QModelIndex(), pos, pos+count-1);

	for (int i = 0; i < count; ++i)
		tasks->removeAt(pos);

	endRemoveRows();
	return true;
}

bool TaskModel::removeRow(int pos, const QModelIndex&) {
	return removeRows(pos, 1);
}

void TaskModel::save() {
	QSettings settings("TaskTimer", "tasktimer");
	settings.clear();

	QDir dir(QDir::homePath());
	dir.mkdir(".tasktimer");

	int i = 0;
	foreach(Task *j, *tasks) {
		settings.beginGroup(QString("task%1").arg(QString::number(i), 4, '0'));
		settings.setValue("name", j->name());
		settings.setValue("totalElapsed", j->totalElapsed());
		settings.setValue("sessionElapsed", j->sessionElapsed());
		settings.setValue("status", j->isDone());
		settings.setValue("priority", j->priority());
		settings.setValue("group", j->group());
		settings.setValue("workTimes", j->getWorkTimesString());

		QFile file(QDir::homePath()+"/.tasktimer/"+QString("task%1").arg(QString::number(i), 4, '0'));
		if (file.open(QIODevice::WriteOnly)) {
			file.write(j->note().toUtf8());
		}

		settings.endGroup();
		++i;
	}
}

void TaskModel::load() {
	QSettings settings("TaskTimer", "tasktimer");

	foreach(QString group, settings.childGroups()) {
		settings.beginGroup(group);

		Task *j = new Task(settings.value("name").toString(), this);
		j->setTotalElapsed(settings.value("totalElapsed").toUInt());
		j->setSessionElapsed(settings.value("sessionElapsed").toUInt());
		j->setDone(settings.value("status").toBool());
		j->setPriority(settings.value("priority").toInt());
		j->setGroup(settings.value("group").toString());
		j->setWorkTimesString(settings.value("workTimes").toString());

		QFile file(QDir::homePath()+"/.tasktimer/"+group);
		if (file.exists() && file.open(QIODevice::ReadOnly)) {
			j->setNote(QString(file.readAll()));
		}

		tasks->append(j);

		settings.endGroup();
	}
}

bool TaskModel::hasActive() const {
	foreach(Task *j, *tasks) {
		if (j->isStarted())
			return true;
	}
	return false;
}

bool TaskModel::isActive(const QModelIndex &index) const {
	return tasks->at(index.row())->isStarted();
}

Qt::DropActions TaskModel::supportedDropActions() const
{
	return  Qt::CopyAction| Qt::MoveAction;
}

Qt::DropActions TaskModel::supportedDragActions() const
{
	return  Qt::CopyAction | Qt::MoveAction;
}

bool TaskModel::isDone(const QModelIndex &index) const {
	return tasks->at(index.row())->isDone();
}

void TaskModel::setDone(bool d, const QModelIndex &index) {
	tasks->at(index.row())->stop();
	tasks->at(index.row())->setDone(d);
	emit dataChanged(createIndex(0, 0), createIndex(2, tasks->count()-1));
}

QModelIndex TaskModel::index (int row, int column, const QModelIndex &/*parent*/) const {
	return createIndex(row, column, tasks->at(row));
}

void TaskModel::stopAll() {
	foreach(Task *j, *tasks) {
		j->stop();
	}
	emit dataChanged(index(0, TotalTime), index(tasks->count()-1, SessionTime));
}

void TaskModel::revertActive(uint t) {
	foreach(Task *j, *tasks) {
			j->revert(t);
	}
	emit dataChanged(index(0, TotalTime), index(tasks->count()-1, SessionTime));
}

int TaskModel::priority(const QModelIndex& index) const {
	return tasks->at(index.row())->priority();
}

void TaskModel::setPriority(int value, const QModelIndex& index) {
	tasks->at(index.row())->setPriority(value);
}

void TaskModel::startNewSession(const QModelIndex &index) {
	tasks->at(index.row())->newSession();
	emit dataChanged(this->index(0, TotalTime), this->index(tasks->count()-1, SessionTime));
}

Task* TaskModel::getTask(const QModelIndex &index) {
	return tasks->at(index.row());
}
