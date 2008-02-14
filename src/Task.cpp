#include <QDebug>
#include <QFile>
#include "Task.h"

int Task::nextId = 0;

Task::Task(QString n, QObject *parent) : QObject(parent) {
	m_name = n;
	m_id = nextId;
	Task::nextId++;
	m_totalElapsedTime = 0;
	m_sessionElapsedTime = 0;
	m_started = NULL;
	m_done = false;
	m_priority = 0;
}

Task::~Task() {
	stop();
}

void Task::start() {
	if (m_started)
		stop();
	else {
		m_started = new QDateTime(QDateTime::currentDateTime());
		m_done = false;
		logStatus(Started);
	}
}

void Task::stop() {
	if (m_started) {
		uint ctime = calculateElapsedTime();
		m_totalElapsedTime += ctime;
		m_sessionElapsedTime += ctime;
		delete m_started;
		m_started = NULL;
		logStatus(Stopped);
	}
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

QString Task::note() {
	return m_note;
}

void Task::setNote(QString note) {
	m_note = note;
}

void Task::logStatus(NextStatus s) {
	QFile log (QString("/tmp/task_%1.log").arg(m_id));
	log.open(QIODevice::Append);
	QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

	if (s == Started) {
		log.write(QString("%1 Task started\n").arg(date).arg(m_id).toAscii());
	}
	else {
		log.write(QString("%1 Task stopped\n").arg(date).arg(m_id).toAscii());
	}
	log.close();
}
