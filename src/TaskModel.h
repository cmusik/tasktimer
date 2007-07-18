#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QList>
#include <QAbstractTableModel>
#include "Task.h"

enum Columns {
	Counter = 0,
	Priority = 1,
	Name = 2,
	ColumnTime = 3
};

enum Roles {
	Active = 32,
	Done = 33
};

class TaskModel : public QAbstractTableModel {
	Q_OBJECT

	public:
		TaskModel(QObject* = 0);

		QList<Task*> *tasks;

		void start(const QModelIndex&);
		void stop(const QModelIndex&);
		bool insertRows(int, int, const QModelIndex& = QModelIndex());
		bool insertRow(int, const QModelIndex& = QModelIndex());
		bool removeRows(int, int, const QModelIndex& = QModelIndex());
		bool removeRow(int, const QModelIndex& = QModelIndex());
		bool hasActive() const;
		bool isActive(const QModelIndex&) const;
		bool isDone(const QModelIndex&) const;
		void setDone(bool, const QModelIndex&);
		QModelIndex index (int, int, const QModelIndex & = QModelIndex() ) const;
		void revertActive(uint);
		int priority(const QModelIndex&) const;
		void setPriority(int, const QModelIndex&);
		int rowCount(const QModelIndex&) const;
		int columnCount(const QModelIndex&) const;

	public slots:
		void save();
		void load();
		void stopAll();

	private slots:
		void updateData();

	private:
		QVariant data(const QModelIndex&, int) const;
		QVariant headerData (int, Qt::Orientation, int) const;
		Qt::ItemFlags flags(const QModelIndex&) const;
		bool setData(const QModelIndex&, const QVariant&, int = Qt::EditRole);
		Qt::DropActions supportedDropActions() const;
		Qt::DropActions supportedDragActions() const;
};

#endif
