#include <QDebug>
#include "Task.h"

Task::Task(QString n, QObject *parent) : QObject(parent) {
	m_name = n;
	m_totalElapsedTime = 0;
	m_sessionElapsedTime = 0;
	m_started = NULL;
	m_done = false;
	m_priority = 0;
}

void Task::start() {
	if (m_started)
		stop();
	else {
		m_started = new QDateTime(QDateTime::currentDateTime());
		m_done = false;
	}
}

void Task::stop() {
	uint ctime = calculateElapsedTime();
	m_totalElapsedTime += ctime;
	m_sessionElapsedTime += ctime;
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

uint Task::totalElapsed() {
	return m_totalElapsedTime+calculateElapsedTime();
}

uint Task::sessionElapsed() {
	return m_sessionElapsedTime+calculateElapsedTime();
}

void Task::setTotalElapsed(uint t) {
	m_totalElapsedTime = t;
}

void Task::setSessionElapsed(uint t) {
	m_sessionElapsedTime = t;
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
	if (isStarted()) {
		stop();
		m_totalElapsedTime -= t;
		m_sessionElapsedTime -= t;
		start();
	}
}

void Task::addSessionTime(int t) {
	bool started = isStarted();
	stop();

	if (t < 0 && (uint) (-t) > m_totalElapsedTime)
		m_totalElapsedTime = 0;
	else
		m_totalElapsedTime += t;

	if (t < 0 && (uint) (-t) > m_sessionElapsedTime)
		m_sessionElapsedTime = 0;
	else
		m_sessionElapsedTime += t;

	if (started)
		start();
}

void Task::addTotalTime(int t) {
	bool started = isStarted();
	stop();

	if (t < 0 && (uint) (-t) > m_totalElapsedTime)
		m_totalElapsedTime = 0;
	else
		m_totalElapsedTime += t;

	if (started)
		start();
}

int Task::priority() const {
	return m_priority;
}

void Task::setPriority(int p) {
	m_priority = p;
}

void Task::newSession() {
	bool started = isStarted();
	stop();
	m_sessionElapsedTime = 0;
	if (started)
		start();
}
