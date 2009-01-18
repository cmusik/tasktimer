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
		TaskTime(TaskTimeType, QDateTime*, QObject* = NULL);

		QDateTime* time();
		TaskTimeType type();

	private:
		TaskTimeType m_type;
		QDateTime *m_time;
};


#endif

