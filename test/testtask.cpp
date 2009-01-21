#include <QtTest/QtTest>
#include <QDebug>
#include "../src/Task.h"
#include "../src/TaskTime.h"

class TestTask: public QObject {
	Q_OBJECT

	private:
		Task *m_task;


	private slots:
		void init();
		void cleanup();
		void testDone();
		void testName();
		void testStart();
		void testPriority();
};

void TestTask::init() {
	m_task = new Task("tasksname", this);
	QVERIFY(m_task->inherits("Task") == true);
}

void TestTask::cleanup() {
	delete m_task;
	m_task = NULL;
}

void TestTask::testDone() {
	QVERIFY(m_task->isDone() == false);
	QDateTime d = QDateTime::currentDateTime();
	m_task->setDone(d);
	QVERIFY(m_task->isDone() == true);
	QVERIFY(m_task->dateFinished() == d);

	m_task->start();
	m_task->stop();
	QVERIFY(m_task->isDone() == false);
}

void TestTask::testName() {
	QVERIFY(m_task->name() == "tasksname");
	m_task->setName(QString("new tasksname"));
	QVERIFY(m_task->name() == "new tasksname");
}

void TestTask::testStart() {
	QCOMPARE(m_task->isStarted(), false);
	QVERIFY(m_task->totalElapsed() == 0);
	QVERIFY(m_task->sessionElapsed() == 0);

	m_task->start();

	QCOMPARE(m_task->isStarted(), true);
	QWARN("Waiting 3 seconds");
	QTest::qSleep(3000);
	m_task->stop();

	QCOMPARE(m_task->isStarted(), false);
	QVERIFY(m_task->totalElapsed() == 3);
	QVERIFY(m_task->sessionElapsed() == 3);

	m_task->newSession();

	m_task->start();
	QWARN("Waiting another 3 seconds");
	QTest::qSleep(3000);
	m_task->stop();
	QVERIFY(m_task->totalElapsed() == 6);
	QVERIFY(m_task->sessionElapsed() == 3);

	QVERIFY(m_task->calculateTime(QDateTime(), QDateTime()) == 6);
}

void TestTask::testPriority() {
	QVERIFY(m_task->priority() == 1);
	m_task->setPriority(0);
	QVERIFY(m_task->priority() == 1);
	m_task->setPriority(2);
	QVERIFY(m_task->priority() == 2);
	m_task->setPriority(3);
	QVERIFY(m_task->priority() == 3);
	m_task->setPriority(4);
	QVERIFY(m_task->priority() == 3);
}

int main (int args, char *argv[]) {
	TestTask obj;
	QTest::qExec(&obj, args, argv);
}
#include "testtask.moc"
