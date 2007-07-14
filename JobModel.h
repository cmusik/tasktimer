#ifndef JOBMODEL_H
#define JOBMODEL_H

#include <QList>
#include <QAbstractTableModel>
#include "Job.h"

enum Columns {
	Counter = 0,
	Name = 1,
	Time = 2
};

class JobModel : public QAbstractTableModel {
	Q_OBJECT

	public:
		JobModel(QObject* = 0);

		QList<Job*> *jobs;

		void start(const QModelIndex&);
		void stop(const QModelIndex&);
		bool insertRows(int, int, const QModelIndex& = QModelIndex());
		bool insertRow(int, const QModelIndex& = QModelIndex());
		bool removeRows(int, int, const QModelIndex& = QModelIndex());
		bool removeRow(int, const QModelIndex& = QModelIndex());
		bool hasActive();
		bool isActive(const QModelIndex&);
		bool isDone(const QModelIndex&);
		void setDone(bool, const QModelIndex&);
		QModelIndex index (int, int, const QModelIndex & = QModelIndex() ) const;

	public slots:
		void save();
		void load();

	private slots:
		void updateData();

	private:
		int rowCount(const QModelIndex&) const;
		int columnCount(const QModelIndex&) const;
		QVariant data(const QModelIndex&, int) const;
		QVariant headerData (int, Qt::Orientation, int) const;
		Qt::ItemFlags flags(const QModelIndex&) const;
		bool setData(const QModelIndex&, const QVariant&, int = Qt::EditRole);
		Qt::DropActions supportedDropActions() const;
		Qt::DropActions supportedDragActions() const;
};

#endif
