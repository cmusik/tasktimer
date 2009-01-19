#ifndef  TASKTIME_INC
#define  TASKTIME_INC

#include <QObject>
#include <QDateTime>

enum TaskTimeType {
	Start,
	Stop
};

class TaskTime : public QObject {
	Q_OBJECT

	public:
		TaskTime(QObject* = NULL);
		TaskTime(TaskTimeType, QDateTime*, int=0, QObject* = NULL);

		QDateTime* time();
		TaskTimeType type();
		void correct(int);
		int getCorrectTime();

	private:
		TaskTimeType m_type;
		QDateTime *m_time;
		int m_correct;
};


#endif

