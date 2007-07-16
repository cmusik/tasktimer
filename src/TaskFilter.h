#ifndef TASKFILTER_H
#define TASKFILTER_H

#include <QSortFilterProxyModel>

class TaskFilter : public QSortFilterProxyModel {
	Q_OBJECT

	public:
		TaskFilter(QObject * = 0);
		bool filterAcceptsRow(int, const QModelIndex&) const;

	public slots:
		void filterDone(bool);
		void invalidate();

	private:
		bool m_filterDone;
};

#endif
