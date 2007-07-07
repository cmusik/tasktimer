#include <QLineEdit>
#include <QModelIndex>
#include <QDebug>
#include <QPainter>
#include "JobEdit.h"

JobEdit::JobEdit(QObject *parent) : QItemDelegate(parent) {
}

QWidget* JobEdit::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const {

	if (index.column() == 1)
		return new QLineEdit(parent);

	return NULL;
}

void JobEdit::setEditorData(QWidget *editor, const QModelIndex &index) const {
	QLineEdit *spinBox = static_cast<QLineEdit*>(editor);

	if (index.column() == 1) {
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

	QString value = spinBox->text();

	model->setData(index, value);
}

void JobEdit::updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex &index) const {
	editor->setGeometry(option.rect);
}

void JobEdit::paint (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const {
	/*
	if (index.column() == 0) {
	//if (option.state & QStyle::State_Selected)
		//painter->fillRect(option.rect, option.palette.highlight());

	painter->save();
	//painter->setRenderHint(QPainter::Antialiasing, true);
	//painter->setPen(Qt::NoPen);
	//
	//if (option.state & QStyle::State_Selected)
	//	painter->setBrush(option.palette.highlightedText());
	//else
	//	painter->setBrush(QBrush(Qt::black));

	painter->drawText(option.rect.x()+5, option.rect.y()+((option.rect.height()+option.fontMetrics.xHeight())/2), "asdf");
	painter->restore();

	}
	else
	*/
		QItemDelegate::paint(painter, option, index);
}

QSize JobEdit::sizeHint (const QStyleOptionViewItem &option, const QModelIndex &index) const {
	return QItemDelegate::sizeHint(option, index);
}
