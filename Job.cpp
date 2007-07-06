#include <QDebug>
#include "Job.h"

Job::Job(QObject *parent, QString n) : QObject(parent) {
	name = n;
	elapsedTime = 0;
	started = NULL;
}

void Job::start() {
	if (!started)
		started = new QDateTime(QDateTime::currentDateTime());
}

void Job::stop() {
	elapsedTime += calculateElapsedTime();
	delete started;
	started = NULL;
}

void Job::output() {
}

QString Job::getName() {
	return name;
}

uint Job::calculateElapsedTime() {
	uint e = 0;

	if (started && started->isValid()) {
		QDateTime *now = new QDateTime(QDateTime::currentDateTime());
		e = now->toTime_t() - started->toTime_t();

		delete now;
		now = NULL;
	}
	return e;
}

uint Job::duration() {
	return elapsedTime+calculateElapsedTime();
}
