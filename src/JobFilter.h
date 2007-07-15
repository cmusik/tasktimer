#ifndef JOBFILTER_H
#define JOBFILTER_H

#include <QSortFilterProxyModel>

class JobFilter : public QSortFilterProxyModel {
	Q_OBJECT

	public:
		JobFilter(QObject * = 0);
		bool filterAcceptsRow(int, const QModelIndex&) const;

	public slots:
		void filterDone(bool);

	private:
		bool m_filterDone;
};

#endif
