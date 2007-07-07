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
	else {
		return QTime().addSecs(jobs->at(index.row())->duration()).toString("HH:mm:ss");
	}

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

bool JobModel::insertRows(int pos, int count, const QModelIndex&) {
	qDebug() <<  pos+1 << pos+count;
	beginInsertRows(QModelIndex(), pos+1, pos+count);

	for (int i = 0; i < count; ++i)
		jobs->insert(pos+1, new Job(this, QString()));

	endInsertRows();
	return true;
}

bool JobModel::removeRows(int pos, int count, const QModelIndex&) {
	qDebug() <<  pos << pos+count;
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
			out << j->getName() << " " << j->duration() << "\n";
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
			QString time = list.takeLast();

			Job *j = new Job(this, list.join(" "));
			j->setElapsed(time.toUInt());
			jobs->append(j);
		}

		file.close();
	}
}
