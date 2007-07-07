#include <QVariant>
#include <QDebug>
#include <unistd.h>
#include <QTimer>
#include "JobModel.h"
#include "Job.h"

JobModel::JobModel(QObject *parent) : QAbstractTableModel(parent) {
	jobs = new QList<Job*>();

	Job *j1 = new Job(this, "job1");
	Job *j2 = new Job(this, "job2");

	j1->start();
	j2->start();

	j1->stop();
	j2->stop();

	jobs->append(j1);
	jobs->append(j2);

	QTimer *timer = new QTimer(this);
	timer->setInterval(1000);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateData()));
	timer->start();
}

void JobModel::updateData() {
	emit dataChanged(createIndex(1, 0), createIndex(1, jobs->count()));
}

int JobModel::rowCount(const QModelIndex&) const {
	return jobs->count();
}

int JobModel::columnCount(const QModelIndex&) const {
	return 2;
}

QVariant JobModel::data(const QModelIndex& index, int role) const {
	if (role != Qt::DisplayRole)
		return QVariant();

	if (!index.isValid())
		return QVariant();

	if (index.column() == 0)
		return jobs->at(index.row())->getName();
	else
		return jobs->at(index.row())->duration();

}

void JobModel::start(const QModelIndex& index) {
	jobs->at(index.row())->start();
}

void JobModel::stop(const QModelIndex& index) {
	jobs->at(index.row())->stop();
}

QVariant JobModel::headerData (int section, Qt::Orientation orientation, int role) const {
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Horizontal) {
		switch(section) {
			case 0:
				return QString("Name");
			case 1:
				return QString("Time");
		}
	}
	else {
		return section+1;
	}

	return QVariant();
}

Qt::ItemFlags JobModel::flags(const QModelIndex& index) const {
	if (!index.isValid())
		return NULL;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool JobModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if (!index.isValid())
		return false;
	if (index.column() != 0)
		return false;

	jobs->at(index.row())->setName(value.toString());
	return true;
}

bool JobModel::insertRows(int, int count, const QModelIndex&) {
	qDebug() <<  jobs->count() << jobs->count()+count-1;
	beginInsertRows(QModelIndex(), jobs->count(), jobs->count()+count-1);

	for (int i = 0; i < count; ++i)
		jobs->append(new Job(this, QString()));

	endInsertRows();
	return true;
}
