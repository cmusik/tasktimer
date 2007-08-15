#include <QLineEdit>
#include <QModelIndex>
#include <QDebug>
#include <QPainter>
#include <QIntValidator>
#include "TaskEdit.h"
#include "TaskModel.h"

static int objectHeight = 14;
static const QPointF points[3] = {QPointF(0.0, 0.0), QPointF(10.0, (objectHeight/2)), QPointF(0.0, objectHeight)};

TaskEdit::TaskEdit(QObject *parent) : QItemDelegate(parent) {
}

QWidget* TaskEdit::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const {
	switch(index.column()) {
		case Name:
			return new QLineEdit(parent);
		case TotalTime:
		case SessionTime:
			{
			QLineEdit *lineEdit = new QLineEdit(parent);
			const QIntValidator *v = new QIntValidator(parent);
			lineEdit->setValidator(v);
			return lineEdit;
			}
		default:
			return NULL;
	}
}

void TaskEdit::setEditorData(QWidget *editor, const QModelIndex &index) const {

	switch(index.column()) {
		case Name:
			{
				QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
				QString value = index.model()->data(index, Qt::DisplayRole).toString();
				lineEdit->setText(value);
			}
		default:
			return;
	}
}

void TaskEdit::setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const {
	switch(index.column()) {
		case Name:
		case TotalTime:
		case SessionTime:
			{
				QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
				QString value = lineEdit->text();
				model->setData(index, value);
			}
	}
}

void TaskEdit::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const {
	editor->setGeometry(option.rect);
}

void TaskEdit::paint (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const {
	painter->save();

	bool active = index.model()->data(index, Active).toBool();
	bool done = index.model()->data(index, Done).toBool();
	int priority = index.model()->data(index).toInt();

	painter->setRenderHint(QPainter::Antialiasing, true);
	if (option.state & QStyle::State_Selected) {
		painter->fillRect(option.rect, option.palette.highlight());
		painter->setPen(option.palette.highlightedText().color());
	}

	if (done) {
		QFont f = option.font;
		f.setStrikeOut(true);
		painter->setFont(f);
	}

	painter->translate(option.rect.x(), option.rect.y() );

	float h = option.rect.height();

	if (index.column() == Priority) {
		if (option.state & QStyle::State_Selected)
			painter->setPen(QColor(Qt::white));
		else
			painter->setPen(QColor(Qt::black));

		int barh = h/7;
		for(int i = 0; i < priority; i++) {
			QColor c;
			switch(priority) {
				case 1:
					c = QColor(Qt::green);
					break;
				case 2:
					c = QColor(Qt::yellow);
					break;
				case 3:
					c = QColor(Qt::red);
					break;
			}
			painter->drawRect(5, h-2*(barh+(barh*i)), option.rect.width()-10, barh);
			painter->fillRect(5, h-2*(barh+(barh*i)), option.rect.width()-10, barh, QBrush(c));
		}
	}
	else if (index.column() == Name) {
		painter->save();
		painter->translate(5, (h-objectHeight)/2);
		if (active) {
			if (option.state & QStyle::State_Selected)
				painter->setPen(QColor(Qt::white));
			else
				painter->setPen(QColor(Qt::black));
			painter->setBrush(QBrush(Qt::green));
			painter->drawPolygon(points, 3);
		}
		painter->restore();

		painter->drawText(20, ((option.rect.height()+option.fontMetrics.xHeight())/2), index.model()->data(index).toString());
	}
	else {
		int filler = 5;
		if (index.column() == TotalTime || index.column() == SessionTime) {
			int ow = option.fontMetrics.width(index.model()->data(index).toString());
			filler = option.rect.width() - ow - 5;
		}
		painter->drawText(filler, (h+option.fontMetrics.xHeight())/2, index.model()->data(index).toString());
	}
	painter->restore();
}

QSize TaskEdit::sizeHint (const QStyleOptionViewItem &option, const QModelIndex &index) const {
	return QItemDelegate::sizeHint(option, index);
}
