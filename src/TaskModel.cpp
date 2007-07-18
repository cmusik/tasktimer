#include <QVariant>
#include <QDebug>
#include <unistd.h>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QSettings>
#include "TaskModel.h"
#include "Task.h"


TaskModel::TaskModel(QObject *parent) : QAbstractTableModel(parent) {
	tasks = new QList<Task*>();

	QTimer *timer = new QTimer(this);
	timer->setInterval(1000);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateData()));
	timer->start();
}

void TaskModel::updateData() {
	for(int i = 0; i < tasks->count(); ++i) {
		if (tasks->at(i)->isStarted()) {
			emit dataChanged(index(i, ColumnTime), index(i, ColumnTime));
		}
	}
}

int TaskModel::rowCount(const QModelIndex&) const {
	return tasks->count();
}

int TaskModel::columnCount(const QModelIndex&) const {
	return 4;
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
		case ColumnTime:
			return QTime().addSecs(tasks->at(index.row())->duration()).toString("HH:mm:ss");
	}
	return QVariant();

}

void TaskModel::start(const QModelIndex& index) {
	tasks->at(index.row())->start();
	tasks->at(index.row())->setDone(false);
	emit dataChanged(this->index(0, 0), this->index(tasks->count()-1, 3));
}

void TaskModel::stop(const QModelIndex& index) {
	tasks->at(index.row())->stop();
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
			case ColumnTime:
				return QString("Time");
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
		case ColumnTime:
			tasks->at(index.row())->addTime(value.toInt()*60);
			break;
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

	int i = 0;
	foreach(Task *j, *tasks) {
		settings.beginGroup(QString("task%1").arg(QString::number(i), 4, '0'));
		settings.setValue("name", j->name());
		settings.setValue("duration", j->duration());
		settings.setValue("status", j->isDone());
		settings.setValue("priority", j->priority());
		settings.endGroup();
		++i;
	}
}

void TaskModel::load() {
	QSettings settings("TaskTimer", "tasktimer");

	foreach(QString group, settings.childGroups()) {
		settings.beginGroup(group);

		Task *j = new Task(settings.value("name").toString(), this);
		j->setElapsed(settings.value("duration").toUInt());
		j->setDone(settings.value("status").toBool());
		j->setPriority(settings.value("priority").toInt());
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
	emit dataChanged(index(0, ColumnTime), index(tasks->count(), ColumnTime));
}

void TaskModel::revertActive(uint t) {
	foreach(Task *j, *tasks) {
			j->revert(t);
	}
	emit dataChanged(index(0, ColumnTime), index(tasks->count(), ColumnTime));
}

int TaskModel::priority(const QModelIndex& index) const {
	return tasks->at(index.row())->priority();
}

void TaskModel::setPriority(int value, const QModelIndex& index) {
	tasks->at(index.row())->setPriority(value);
}
