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
		void setDone(QDateTime = QDateTime(QDateTime::currentDateTime()));
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
		QDateTime dateCreated();
		void setDateCreated(QDateTime);
		QDateTime dateFinished();
		void setDateFinished(QDateTime);
        bool isSaved();
        void markSaved();

	private:
		uint calculateElapsedTime();
		void logStatus(NextStatus);

		uint m_totalElapsedTime;
		uint m_sessionElapsedTime;
		int m_priority;
		int m_id;
		QString m_name;
		QDateTime *m_started;
		QDateTime m_dateCreated;
		QDateTime m_dateFinished;
		QList<TaskTime*> *m_times;
		QString m_group;
		QString m_note;
        bool m_saved;
		static int nextId;
};

#endif
