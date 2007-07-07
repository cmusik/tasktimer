#include <QLineEdit>
#include <QModelIndex>
#include <QDebug>
#include "JobEdit.h"

JobEdit::JobEdit(QObject *parent) : QItemDelegate(parent) {
}

QWidget* JobEdit::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const {

	if (index.column() == 0)
		return new QLineEdit(parent);

	return NULL;
}

void JobEdit::setEditorData(QWidget *editor, const QModelIndex &index) const {
	QLineEdit *spinBox = static_cast<QLineEdit*>(editor);

	if (index.column() == 0) {
		QString value = index.model()->data(index, Qt::DisplayRole).toString();

		spinBox->setText(value);
	}
	else {
		int value = index.model()->data(index, Qt::DisplayRole).toInt();

		spinBox->setText(QString::number(value));
	}
}

void JobEdit::setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const {
	QLineEdit *spinBox = static_cast<QLineEdit*>(editor);
	//spinBox->interpretText();
	QString value = spinBox->text();

	model->setData(index, value);
}

void JobEdit::updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex &index) const {
	editor->setGeometry(option.rect);
}
