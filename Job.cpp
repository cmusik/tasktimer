#include <QDebug>
#include "Job.h"

Job::Job(QObject *parent, QString n) : QObject(parent) {
	progress = new QStack<JobTime*>();
	name = n;
}

void Job::start() {
	//if (progress->count() && progress->top()->isStopped())
		progress->push(new JobTime(this));
}

void Job::stop() {
	progress->top()->stop();
}

void Job::output() {
	for (int i = 0; i < progress->size(); ++i) {
		qDebug() << *progress->at(i)->started() << *progress->at(i)->stopped();
	}
}

QString Job::getName() {
	return name;
}

uint Job::duration() {
	uint all = 0;
	for (int i = 0; i < progress->size(); ++i) {
		JobTime *j = progress->at(i);
		QDateTime *stopped = j->stopped();
		QDateTime *started = j->started();

		if (stopped == NULL)
			stopped = new QDateTime(QDateTime::currentDateTime());

		all += stopped->toTime_t() - started->toTime_t();
	}
	return all;
}
