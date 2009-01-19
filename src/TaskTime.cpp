#include <QDebug>
#include "TaskTime.h"

TaskTime::TaskTime(QObject *p) : QObject(p) {
	m_correct = 0;
}

TaskTime::TaskTime(TaskTimeType t, QDateTime *d, int c, QObject *p) : QObject(p) {
	m_time = d;
	m_type = t;
	m_correct = c;
}

QDateTime* TaskTime::time() {
	return m_time;
}

TaskTimeType TaskTime::type() {
	return m_type;
}

void TaskTime::correct(int c) {
	qDebug() << c << m_correct;
	m_correct += c;
}

int TaskTime::getCorrectTime() {
	return m_correct;
}
