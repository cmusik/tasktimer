#include <QVariant>
#include <QDebug>
#include <unistd.h>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include "JobModel.h"
#include "Job.h"


JobModel::JobModel(QObject *parent) : QAbstractTableModel(parent) {
	jobs = new QList<Job*>();

	QTimer *timer = new QTimer(this);
	timer->setInterval(1000);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateData()));
	timer->start();
}

void JobModel::updateData() {
	for(int i = 0; i < jobs->count(); ++i) {
		if (jobs->at(i)->isStarted()) {
			emit dataChanged(index(i, Time), index(i, Time));
		}
	}
}

int JobModel::rowCount(const QModelIndex&) const {
	return jobs->count();
}

int JobModel::columnCount(const QModelIndex&) const {
	return 3;
}

QVariant JobModel::data(const QModelIndex& index, int role) const {
	if (role != Qt::DisplayRole)
		return QVariant();

	if (!index.isValid())
		return QVariant();

	switch(index.column()) {
		case Counter:
			return index.row()+1;
		case Name:
			return jobs->at(index.row())->getName();
		case Time:
			return QTime().addSecs(jobs->at(index.row())->duration()).toString("HH:mm:ss");
	}
	return QVariant();

}

void JobModel::start(const QModelIndex& index) {
	jobs->at(index.row())->start();
	jobs->at(index.row())->setDone(false);
	emit dataChanged(createIndex(0, 0), createIndex(2, jobs->count()-1));
}

void JobModel::stop(const QModelIndex& index) {
	jobs->at(index.row())->stop();
	emit dataChanged(createIndex(0, 0), createIndex(2, jobs->count()-1));
}

QVariant JobModel::headerData (int section, Qt::Orientation orientation, int role) const {
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Horizontal) {
		switch(section) {
			case Counter:
				return QVariant();
			case Name:
				return QString("Name");
			case Time:
				return QString("Time");
		}
	}
	else {
		return section+1;
	}

	return QVariant();
}

Qt::ItemFlags JobModel::flags(const QModelIndex& index) const {
	Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);

	if (index.isValid())
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
	else
		return Qt::ItemIsDropEnabled | defaultFlags;

}

bool JobModel::setData(const QModelIndex &index, const QVariant &value, int /*role*/) {
	if (!index.isValid())
		return false;
	if (index.column() != Name)
		return false;

	jobs->at(index.row())->setName(value.toString());
	return true;
}

bool JobModel::insertRows(int pos, int count, const QModelIndex&) {
	beginInsertRows(QModelIndex(), pos+1, pos+count);

	for (int i = 0; i < count; ++i)
		jobs->insert(pos+1, new Job(this, QString()));

	endInsertRows();
	return true;
}

bool JobModel::removeRows(int pos, int count, const QModelIndex&) {
	beginRemoveRows(QModelIndex(), pos, pos+count-1);

	for (int i = 0; i < count; ++i)
		jobs->removeAt(pos);

	endRemoveRows();
	return true;
}

void JobModel::save() {
	QFile file("saved_jobs.txt");
	if (file.open(QFile::WriteOnly | QFile::Truncate)) {
		QTextStream out(&file);
		foreach(Job *j, *jobs) {
			out << j->getName() << " " << j->duration() << " " << (j->isDone() ? 1 : 0) << "\n";
		}
		file.close();
	}
}

void JobModel::load() {
	QFile file("saved_jobs.txt");
	if (file.open(QFile::ReadOnly)) {
		QTextStream in(&file);
		while(!in.atEnd()) {
			QString str = in.readLine();
			QStringList list = str.split(" ");
			QString done = list.takeLast();
			QString time = list.takeLast();

			Job *j = new Job(this, list.join(" "));
			j->setElapsed(time.toUInt());
			j->setDone(done == "1" ? true : false);
			jobs->append(j);
		}

		file.close();
	}
}

bool JobModel::hasActive() {
	foreach(Job *j, *jobs) {
		if (j->isStarted())
			return true;
	}
	return false;
}

bool JobModel::isActive(const QModelIndex &index) {
	return jobs->at(index.row())->isStarted();
}

Qt::DropActions JobModel::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions JobModel::supportedDragActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}

bool JobModel::isDone(const QModelIndex &index) {
	return jobs->at(index.row())->isDone();
}

void JobModel::setDone(bool d, const QModelIndex &index) {
	jobs->at(index.row())->stop();
	jobs->at(index.row())->setDone(d);
	emit dataChanged(createIndex(0, 0), createIndex(2, jobs->count()-1));
}

QModelIndex JobModel::index (int row, int column, const QModelIndex &parent) const {
	return createIndex(row, column, jobs->at(row));
}
