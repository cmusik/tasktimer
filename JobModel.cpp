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
	sleep(1);
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
	emit dataChanged(index(0, 0), index(0, jobs->count()));
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
	qDebug() << "start job";
	jobs->at(index.row())->start();
}

void JobModel::stop(const QModelIndex& index) {
	qDebug() << "stop job";
	jobs->at(index.row())->stop();
}

Qt::ItemFlags JobModel::flags(const QModelIndex& index) const {
	if (!index.isValid())
		return NULL;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
