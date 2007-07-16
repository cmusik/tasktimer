#include <QDebug>

#include "TaskFilter.h"
#include "TaskModel.h"

TaskFilter::TaskFilter(QObject *parent) : QSortFilterProxyModel(parent) {
	m_filterDone = false;
}

bool TaskFilter::filterAcceptsRow(int sourceRow, const QModelIndex &) const {
	if (!m_filterDone)
		return true;

	TaskModel *model = (TaskModel*) sourceModel();
	QModelIndex index = sourceModel()->index(sourceRow, 0);

	return !model->isDone(index);
}

void TaskFilter::filterDone(bool f) {
	m_filterDone = f;
	//reset();
	invalidateFilter();
}

void TaskFilter::invalidate() {
	invalidateFilter();
}
