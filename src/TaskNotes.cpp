#include "TaskNotes.h"

TaskNotes::TaskNotes(QWidget *parent, QString note) : QDialog(parent) {
	setupUi(this);
	textNotes->setPlainText(note);
}

QString TaskNotes::noteText() {
	return textNotes->toPlainText();
}

