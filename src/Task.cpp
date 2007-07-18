#include <QDebug>
#include "Task.h"

Task::Task(QString n, QObject *parent) : QObject(parent) {
	m_name = n;
	m_elapsedTime = 0;
	m_started = NULL;
	m_done = false;
	m_priority = 0;
}

void Task::start() {
	if (m_started)
		stop();
	else
		m_started = new QDateTime(QDateTime::currentDateTime());
}

void Task::stop() {
	m_elapsedTime += calculateElapsedTime();
	delete m_started;
	m_started = NULL;
}

QString Task::name() const {
	return m_name;
}

void Task::setName(QString n) {
	m_name = n;
}

uint Task::calculateElapsedTime() {
	uint e = 0;

	if (m_started && m_started->isValid()) {
		QDateTime *now = new QDateTime(QDateTime::currentDateTime());
		e = now->toTime_t() - m_started->toTime_t();

		delete now;
		now = NULL;
	}
	return e;
}

uint Task::duration() {
	return m_elapsedTime+calculateElapsedTime();
}

void Task::setElapsed(uint t) {
	m_elapsedTime = t;
}

bool Task::isStarted() const {
	if (m_started != NULL)
		return true;

	return false;
}

bool Task::isDone() const {
	return m_done;
}

void Task::setDone(bool d) {
	m_done = d;
}

void Task::revert(uint t) {
	if (isStarted())
		m_elapsedTime -= t;
}

void Task::addTime(int t) {
	if (t < 0 && (uint) (-t) > m_elapsedTime)
		m_elapsedTime = 0;
	else
		m_elapsedTime += t;
}

int Task::priority() const {
	return m_priority;
}

void Task::setPriority(int p) {
	m_priority = p;
}
