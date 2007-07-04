#include "JobTime.h"

JobTime::JobTime(QObject *parent) : QObject(parent) {
	startTime = NULL;
	stopTime = NULL;
	start();
}

JobTime::~JobTime() {
	if (startTime)
		delete startTime;
	if (stopTime)
		delete stopTime;
}

void JobTime::start() {
	startTime = new QDateTime(QDateTime::currentDateTime());
}

void JobTime::stop() {
	stopTime = new QDateTime(QDateTime::currentDateTime());
}

bool JobTime::isStopped() {
	return stopTime->isNull();
}

QDateTime* JobTime::started() {
	return startTime;
}

QDateTime* JobTime::stopped() {
	return stopTime;
}
