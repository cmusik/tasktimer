#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QString>
#include <QDateTime>

#include "TaskTime.h"

enum NextStatus {
	Started,
	Stopped
};

class Task : public QObject {
	Q_OBJECT

	public:
		Task(QString = "", QObject* = 0);
		~Task();

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
		QString note();
		void setNote(QString);
		QString group();
		void setGroup(QString g);
		QString getWorkTimesString();
		void setWorkTimesString(QString);
		int calculateTime(QDateTime, QDateTime);

	private:
		uint calculateElapsedTime();
		void logStatus(NextStatus);

		uint m_totalElapsedTime;
		uint m_sessionElapsedTime;
		int m_priority;
		int m_id;
		QString m_name;
		QDateTime *m_started;
		QDateTime *m_startDate;
		QDateTime *m_finshedDate;
		QList<TaskTime*> *m_times;
		QString m_group;
		bool m_done;
		QString m_note;
		static int nextId;
};

#endif
