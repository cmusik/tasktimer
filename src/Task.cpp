#include <QDebug>
#include <QFile>
#include <QStringList>
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
	m_priority = 1;
	m_group = "";
	m_times = new QList<TaskTime*>();
}

Task::~Task() {
	stop();
}

void Task::start() {
	if (m_started)
		stop();
	else {
		m_started = new QDateTime(QDateTime::currentDateTime());
		m_times->push_back(new TaskTime(Start, new QDateTime(QDateTime::currentDateTime()), this));
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
		m_times->push_back(new TaskTime(Stop, new QDateTime(QDateTime::currentDateTime()), this));
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

QString Task::group() {
	return m_group;
}

void Task::setGroup(QString g) {
	m_group = g;
}

void Task::logStatus(NextStatus s) {
	QFile log (QString("/tmp/task_%1.log").arg(m_id));
	log.open(QIODevice::Append);
	QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

	if (s == Started) {
		log.write(QString("%1 Task started\n").arg(date).toAscii());
	}
	else {
		log.write(QString("%1 Task stopped\n").arg(date).toAscii());
	}
	log.close();
}

QString Task::getWorkTimesString() {
	QStringList str;
	TaskTime *t;
	foreach(t, *m_times) {
		str << QString::number(t->type()) << QString::number(t->time()->toTime_t());
	}

	if (m_times->count() > 0 && t->type() != Stop) {
		str << QString::number(Stop) << QString::number(QDateTime::currentDateTime().toTime_t());
	}
	return str.join(",");
}

void Task::setWorkTimesString(QString s) {
	QStringList str = s.split(",", QString::SkipEmptyParts);
	if (str.count() > 0) {
		m_times->clear();

		QStringListIterator strIt(str);

		while (strIt.hasNext()) {
			TaskTimeType t = (TaskTimeType) strIt.next().toInt();
			QDateTime *d = new QDateTime(QDateTime::fromTime_t(strIt.next().toUInt()));
			m_times->push_back(new TaskTime(t, d, this));
		}
	}
}
