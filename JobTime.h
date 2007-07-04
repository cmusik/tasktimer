#ifndef JOBTIME_H
#define JOBTIME_H

#include <QObject>
#include <QDateTime>

class JobTime : public QObject {
	Q_OBJECT

	public:
		JobTime(QObject* = NULL);
		~JobTime();

		void start();
		void stop();
		bool isStopped();
		QDateTime* started();
		QDateTime* stopped();

	private:
		QDateTime* startTime;
		QDateTime* stopTime;
};

#endif
