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
	m_priority = 1;
	m_group = "";
	m_times = new QList<TaskTime*>();
	m_dateCreated = QDateTime(QDateTime::currentDateTime());
    m_saved = true;
}

Task::~Task() {
	stop();
}

void Task::start() {
	if (m_started)
		stop();
	else {
		m_started = new QDateTime(QDateTime::currentDateTime());
		m_times->push_back(new TaskTime(Start, new QDateTime(QDateTime::currentDateTime()), 0, this));
		m_dateFinished = QDateTime();
		logStatus(Started);
	}
    m_saved = false;
}

void Task::stop() {
	if (m_started) {
		uint ctime = calculateElapsedTime();
		m_totalElapsedTime += ctime;
		m_sessionElapsedTime += ctime;
		delete m_started;
		m_started = NULL;
		logStatus(Stopped);
		m_times->push_back(new TaskTime(Stop, new QDateTime(QDateTime::currentDateTime()), 0, this));
		calculateTime(QDateTime(),QDateTime() );
	}
}

QString Task::name() const {
	return m_name;
}

void Task::setName(QString n) {
	m_name = n;
    m_saved = false;
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
    m_saved = false;
}

void Task::setSessionElapsed(uint t) {
	m_sessionElapsedTime = t;
    m_saved = false;
}

bool Task::isStarted() const {
	if (m_started != NULL)
		return true;

	return false;
}

bool Task::isDone() const {
	return m_dateFinished.isValid();
}

void Task::setDone(QDateTime d) {
	m_dateFinished = QDateTime(d);
    m_saved = false;
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
	else {
		m_sessionElapsedTime += t;
		if (m_times->count()) {
			m_times->last()->correct(t);
		}
		else {
			m_times->push_back(new TaskTime(Start, new QDateTime(QDateTime::currentDateTime()), 0, this));
			m_times->push_back(new TaskTime(Stop, new QDateTime(QDateTime::currentDateTime()), t, this));
		}
	}

	if (started)
		start();

    m_saved = false;
}

void Task::addTotalTime(int t) {
	bool started = isStarted();
	stop();

	if (t < 0 && (uint) (-t) > m_totalElapsedTime) {
		m_totalElapsedTime = 0;
	}
	else {
		m_totalElapsedTime += t;
		if (m_times->count()) {
			m_times->last()->correct(t);
		}
		else {
			m_times->push_back(new TaskTime(Start, new QDateTime(QDateTime::currentDateTime()), 0, this));
			m_times->push_back(new TaskTime(Stop, new QDateTime(QDateTime::currentDateTime()), t, this));
		}
	}

	if (started)
		start();
}

int Task::priority() const {
	return m_priority;
}

void Task::setPriority(int p) {
	if (p > 0 && p <= 3)
		m_priority = p;

    m_saved = false;
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

    m_saved = false;
}

QString Task::group() {
	return m_group;
}

void Task::setGroup(QString g) {
	m_group = g;

    m_saved = false;
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
	TaskTime *t = NULL;
	foreach(t, *m_times) {
		str << QString::number(t->type()) << QString::number(t->time()->toTime_t()) << QString::number(t->getCorrectTime());;
	}

	if (m_times->count() > 0 && t->type() != Stop) {
		str << QString::number(Stop) << QString::number(QDateTime::currentDateTime().toTime_t()) << QString::number(0);
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
			int c = strIt.next().toInt();
			m_times->push_back(new TaskTime(t, d, c, this));
		}
	}
}

int Task::calculateTime(QDateTime from, QDateTime to) {
	int time = 0;
	QListIterator<TaskTime*> it(*m_times);

	while (it.hasNext()) {
		TaskTime *start = it.next();
		TaskTime *stop = it.next();
		if (!from.isNull() && *start->time() < from)
			continue;

		if (!to.isNull() && *stop->time() >= to)
			continue;
			
			time += stop->time()->toTime_t() - start->time()->toTime_t() + start->getCorrectTime() + stop->getCorrectTime();
	}
	return time;
}

void Task::setDateCreated(QDateTime d) {
	m_dateCreated = d;
    m_saved = false;
}

void Task::setDateFinished(QDateTime d) {
	m_dateFinished = d;
    m_saved = false;
}

QDateTime Task::dateCreated() {
	return m_dateCreated;
}

QDateTime Task::dateFinished() {
	return m_dateFinished;
}

bool Task::isSaved() {
    return m_saved;
}

void Task::markSaved() {
    m_saved = true;
}
