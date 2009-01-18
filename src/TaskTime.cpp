#include "TaskTime.h"

TaskTime::TaskTime(QObject *p) : QObject(p) {
}

TaskTime::TaskTime(TaskTimeType t, QDateTime *d, QObject *p) : QObject(p) {
	m_time = d;
	m_type = t;
}

QDateTime* TaskTime::time() {
	return m_time;
}
TaskTimeType TaskTime::type() {
	return m_type;
}
