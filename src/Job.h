#ifndef JOB_H
#define JOB_H

#include <QObject>
#include <QStack>
#include <QString>
#include <QDateTime>

class Job : public QObject {
	Q_OBJECT

	public:
		Job(QObject* = 0, QString = "");

		void start();
		void stop();
		void output();
		QString getName();
		void setName(QString);
		uint duration();
		void setElapsed(uint);
		bool isStarted();
		bool isDone();
		void setDone(bool);
		void revert(uint);
		void addTime(int);

	private:
		uint calculateElapsedTime();

		uint elapsedTime;
		QString name;
		QDateTime *started;
		bool done;
};

#endif
