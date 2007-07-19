#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QStack>
#include <QString>
#include <QDateTime>

class Task : public QObject {
	Q_OBJECT

	public:
		Task(QString = "", QObject* = 0);

		void start();
		void stop();
		QString name() const;
		void setName(QString);

		uint totalElapsed();
		void setTotalElapsed(uint);

		uint sessionElapsed();
		void setSessionElapsed(uint);

		void newSession();

		bool isStarted() const;
		bool isDone() const;
		void setDone(bool);
		void revert(uint);
		void addSessionTime(int);
		void addTotalTime(int);
		int priority() const;
		void setPriority(int);

	private:
		uint calculateElapsedTime();

		uint m_totalElapsedTime;
		uint m_sessionElapsedTime;
		int m_priority;
		QString m_name;
		QDateTime *m_started;
		bool m_done;
};

#endif
