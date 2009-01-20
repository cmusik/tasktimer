#include <QDebug>
#include "TaskSettings.h"

TaskSettings::TaskSettings(QWidget *p) : QDialog(p) {
	setupUi(this);
	connect(btnAdd, SIGNAL(clicked()), this, SLOT(addGroup()));
	connect(btnRemove, SIGNAL(clicked()), this, SLOT(removeGroup()));
	connect(btnUp, SIGNAL(clicked()), this, SLOT(moveGroupUp()));
	connect(btnDown, SIGNAL(clicked()), this, SLOT(moveGroupDown()));
}
void TaskSettings::addGroup() {
	if (!lineGroup->text().isEmpty()) {
		lstGroups->addItem(lineGroup->text());
		lineGroup->clear();
	}
}

void TaskSettings::removeGroup() {
	foreach(QListWidgetItem *i, lstGroups->selectedItems()) {
		lstGroups->removeItemWidget(i);
		delete i;
	}
}

void TaskSettings::addGroups(QStringList s) {
	lstGroups->addItems(s);
}

QStringList TaskSettings::getGroups() {
	QStringList lst;
	for (int i = 0; i < lstGroups->count(); ++i) {
		lst << lstGroups->item(i)->text();
	}
	return lst;
}

void TaskSettings::moveGroupUp() {
	foreach(QListWidgetItem *i, lstGroups->selectedItems()) {
		int row = lstGroups->row(i);
		lstGroups->takeItem(row);

		if (--row < 0)
			row = 0;

		lstGroups->insertItem(row, i);
		lstGroups->setCurrentRow(row);
	}
}

void TaskSettings::moveGroupDown() {
	foreach(QListWidgetItem *i, lstGroups->selectedItems()) {
		int row = lstGroups->row(i);
		lstGroups->takeItem(row);
		if (++row > lstGroups->count())
			row = lstGroups->count();

		lstGroups->insertItem(row, i);
		lstGroups->setCurrentRow(row);
	}
}
