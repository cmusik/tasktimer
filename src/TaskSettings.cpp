#include <QDebug>
#include "TaskSettings.h"

TaskSettings::TaskSettings(QWidget *p) : QDialog(p) {
	setupUi(this);
	connect(btnAdd, SIGNAL(clicked()), this, SLOT(groupAdd()));
	connect(btnRemove, SIGNAL(clicked()), this, SLOT(groupRemove()));
	connect(btnUp, SIGNAL(clicked()), this, SLOT(groupMoveUp()));
	connect(btnDown, SIGNAL(clicked()), this, SLOT(groupMoveDown()));
	connect(lstGroups, SIGNAL(currentRowChanged(int)), this, SLOT(groupRowChanged(int)));
	connect(lineGroup, SIGNAL(textChanged(QString)), this, SLOT(groupLineChanged(QString)));
}
void TaskSettings::groupAdd() {
	if (!lineGroup->text().isEmpty()) {
		lstGroups->addItem(lineGroup->text());
		lineGroup->clear();
		if (lstGroups->count() > 0)
			btnRemove->setDisabled(false);
		groupRowChanged(lstGroups->currentRow());
	}
}

void TaskSettings::groupRemove() {
	foreach(QListWidgetItem *i, lstGroups->selectedItems()) {
		lstGroups->removeItemWidget(i);
		delete i;
	}
	if (lstGroups->count() == 0)
		btnRemove->setDisabled(true);
}

void TaskSettings::groupAdd(QStringList s) {
	lstGroups->addItems(s);
	if (lstGroups->count() > 0)
		btnRemove->setDisabled(false);
}

QStringList TaskSettings::groupGet() {
	QStringList lst;
	for (int i = 0; i < lstGroups->count(); ++i) {
		lst << lstGroups->item(i)->text();
	}
	return lst;
}

void TaskSettings::groupMoveUp() {
	foreach(QListWidgetItem *i, lstGroups->selectedItems()) {
		int row = lstGroups->row(i);
		lstGroups->takeItem(row);

		if (--row < 0)
			row = 0;

		lstGroups->insertItem(row, i);
		lstGroups->setCurrentRow(row);
	}
}

void TaskSettings::groupMoveDown() {
	foreach(QListWidgetItem *i, lstGroups->selectedItems()) {
		int row = lstGroups->row(i);
		lstGroups->takeItem(row);
		if (++row > lstGroups->count())
			row = lstGroups->count();

		lstGroups->insertItem(row, i);
		lstGroups->setCurrentRow(row);
	}
}

void TaskSettings::groupRowChanged(int r) {
	if (r < 0 || (r == 0 && lstGroups->count() == 1)) {
		btnUp->setDisabled(true);
		btnDown->setDisabled(true);
		return;
	}
	if (r == 0) {
		btnUp->setDisabled(true);
		btnDown->setDisabled(false);
		return;
	}
	if (r == lstGroups->count()-1) {
		btnUp->setDisabled(false);
		btnDown->setDisabled(true);
		return;
	}
	btnUp->setDisabled(false);
	btnDown->setDisabled(false);
}

void TaskSettings::groupLineChanged(QString s) {
	if (s.trimmed().isEmpty())
		btnAdd->setDisabled(true);
	else
		btnAdd->setEnabled(true);
}
