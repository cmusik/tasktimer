#ifndef JOBMODEL_H
#define JOBMODEL_H

#include <QList>
#include <QAbstractTableModel>
#include "Job.h"

class JobModel : public QAbstractTableModel {
	Q_OBJECT

	public:
		JobModel(QObject* = 0);

		QList<Job*> *jobs;

		void start(const QModelIndex&);
		void stop(const QModelIndex&);

	private slots:
		void updateData();

	private:
		int rowCount(const QModelIndex&) const;
		int columnCount(const QModelIndex&) const;
		QVariant data(const QModelIndex&, int) const;
		QVariant headerData (int, Qt::Orientation, int) const;
		Qt::ItemFlags flags(const QModelIndex&) const;
};

#endif
