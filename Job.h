#ifndef JOB_H
#define JOB_H

#include <QObject>
#include <QStack>
#include <QString>

#include "JobTime.h"

class Job : public QObject {
	Q_OBJECT

	public:
		Job(QObject* = 0, QString = "");

		void start();
		void stop();
		void output();
		QString getName();
		uint duration();

	private:
		uint calculateElapsedTime();

		uint elapsedTime;
		QString name;
		QDateTime *started;
};

#endif
