#ifndef JOBEDIT_H
#define JOBEDIT_H

#include <QItemDelegate>

class TaskEdit : public QItemDelegate {
	Q_OBJECT

	public:
		TaskEdit(QObject* = 0);

		QWidget *createEditor(QWidget*, const QStyleOptionViewItem&, const QModelIndex&) const;

		void setEditorData(QWidget*, const QModelIndex&) const;
		void setModelData(QWidget*, QAbstractItemModel*, const QModelIndex&) const;

		void updateEditorGeometry(QWidget*, const QStyleOptionViewItem&, const QModelIndex &) const;
		void paint (QPainter*, const QStyleOptionViewItem&, const QModelIndex&) const;
		QSize sizeHint (const QStyleOptionViewItem&, const QModelIndex&) const;

};

#endif
