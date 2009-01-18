#ifndef TASKEDIT_H
#define TASKEDIT_H

#include <QItemDelegate>
#include <QStringList>

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

		void addGroups(QStringList);
		QStringList getGroups();

	private:
		QStringList m_groups;

};

#endif
