#include <QDebug>
#include "Job.h"

Job::Job(QObject *parent, QString n) : QObject(parent) {
	m_name = n;
	m_elapsedTime = 0;
	m_started = NULL;
	m_done = false;
	m_priority = 0;
}

void Job::start() {
	if (!m_started)
		m_started = new QDateTime(QDateTime::currentDateTime());
}

void Job::stop() {
	m_elapsedTime += calculateElapsedTime();
	delete m_started;
	m_started = NULL;
}

void Job::output() {
}

QString Job::getName() {
	return m_name;
}

void Job::setName(QString n) {
	m_name = n;
}

uint Job::calculateElapsedTime() {
	uint e = 0;

	if (m_started && m_started->isValid()) {
		QDateTime *now = new QDateTime(QDateTime::currentDateTime());
		e = now->toTime_t() - m_started->toTime_t();

		delete now;
		now = NULL;
	}
	return e;
}

uint Job::duration() {
	return m_elapsedTime+calculateElapsedTime();
}

void Job::setElapsed(uint t) {
	m_elapsedTime = t;
}

bool Job::isStarted() {
	if (m_started != NULL)
		return true;

	return false;
}

bool Job::isDone() {
	return m_done;
}

void Job::setDone(bool d) {
	m_done = d;
}

void Job::revert(uint t) {
	if (isStarted())
		m_elapsedTime -= t;
}

void Job::addTime(int t) {
	m_elapsedTime += t;
}

int Job::priority() {
	return m_priority;
}

void Job::setPriority(int p) {
	m_priority = p;
	//m_priority %= 4;
}
