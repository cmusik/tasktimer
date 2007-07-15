#include <QDebug>

#include "JobFilter.h"
#include "JobModel.h"

JobFilter::JobFilter(QObject *parent) : QSortFilterProxyModel(parent) {
	m_filterDone = false;
}

bool JobFilter::filterAcceptsRow(int sourceRow, const QModelIndex &) const {
	if (!m_filterDone)
		return true;

	JobModel *model = (JobModel*) sourceModel();
	QModelIndex index = sourceModel()->index(sourceRow, 0);

	return !model->isDone(index);
}

void JobFilter::filterDone(bool f) {
	m_filterDone = f;
	//reset();
	invalidateFilter();
}
