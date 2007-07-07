#ifndef JOBEDIT_H
#define JOBEDIT_H

#include <QItemDelegate>

class JobEdit : public QItemDelegate {
	Q_OBJECT

	public:
		JobEdit(QObject* = 0);

		QWidget *createEditor(QWidget*, const QStyleOptionViewItem&, const QModelIndex&) const;

		void setEditorData(QWidget*, const QModelIndex&) const;
		void setModelData(QWidget*, QAbstractItemModel*, const QModelIndex&) const;

		void updateEditorGeometry(QWidget*, const QStyleOptionViewItem&, const QModelIndex &) const;

};

#endif
