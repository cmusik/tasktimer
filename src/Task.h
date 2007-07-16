#ifndef JOB_H
#define JOB_H

#include <QObject>
#include <QStack>
#include <QString>
#include <QDateTime>

class Task : public QObject {
	Q_OBJECT

	public:
		Task(QObject* = 0, QString = "");

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
		int priority();
		void setPriority(int);

	private:
		uint calculateElapsedTime();

		uint m_elapsedTime;
		int m_priority;
		QString m_name;
		QDateTime *m_started;
		bool m_done;
};

#endif
