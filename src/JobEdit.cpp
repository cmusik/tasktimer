#include <QLineEdit>
#include <QModelIndex>
#include <QDebug>
#include <QPainter>
#include "JobEdit.h"
#include "JobModel.h"

static int objectHeight = 14;
static const QPointF points[3] = {QPointF(0.0, 0.0), QPointF(10.0, (objectHeight/2)), QPointF(0.0, objectHeight)};

JobEdit::JobEdit(QObject *parent) : QItemDelegate(parent) {
}

QWidget* JobEdit::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const {

	if (index.column() == Name)
		return new QLineEdit(parent);
	if (index.column() == ColumnTime) {
		QLineEdit *lineEdit = new QLineEdit(parent);
		return lineEdit;
	}

	return NULL;
}

void JobEdit::setEditorData(QWidget *editor, const QModelIndex &index) const {

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

void JobEdit::setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const {
	switch(index.column()) {
		case Name:
		case ColumnTime:
			{
				QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
				QString value = lineEdit->text();
				model->setData(index, value);
			}
	}
}

void JobEdit::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const {
	editor->setGeometry(option.rect);
}

void JobEdit::paint (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const {
	painter->save();

	bool active = index.model()->data(index, Active).toBool();
	bool done = index.model()->data(index, Done).toBool();

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

	float x = option.rect.x()+5;
	float h = option.rect.height();
	float y = option.rect.y()+((h-objectHeight)/2);

	if (index.column() == 1) {
		if (active) {
			painter->save();
			painter->translate(x, y);
			//painter->setPen(Qt::NoPen);
			painter->setPen(QColor(Qt::green).darker());
			painter->setBrush(QBrush(Qt::green));
			painter->drawPolygon(points, 3);
			painter->restore();
		}

		painter->drawText(option.rect.x()+20, option.rect.y()+((option.rect.height()+option.fontMetrics.xHeight())/2), index.model()->data(index).toString());
	}
	else
		painter->drawText(option.rect.x()+5, option.rect.y()+((option.rect.height()+option.fontMetrics.xHeight())/2), index.model()->data(index).toString());
	painter->restore();

	//QItemDelegate::paint(painter, option, index);
}

QSize JobEdit::sizeHint (const QStyleOptionViewItem &option, const QModelIndex &index) const {
	return QItemDelegate::sizeHint(option, index);
}
