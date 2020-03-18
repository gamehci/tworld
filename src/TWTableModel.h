/* Copyright (C) 2001-2019 by Madhav Shanbhag and Eric Schmidt.
 * Licensed under the GNU General Public License. No warranty.
 * See COPYING for details.
 */

#include <QAbstractTableModel>

class TWTableModel : public QAbstractTableModel
{
public:
	TWTableModel(QObject* pParent = 0);
	void SetTableSpec(const tablespec* pSpec);

	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

protected:
	struct ItemInfo
	{
		QString sText;
		int align;
	};

	int m_nRows, m_nCols;
	std::vector<ItemInfo> m_vecItems;

	QVariant GetData(int row, int col, int role) const;
};


TWTableModel::TWTableModel(QObject* pParent)
	:
	QAbstractTableModel(pParent),
	m_nRows(0), m_nCols(0)
{
}

void TWTableModel::SetTableSpec(const tablespec* pSpec)
{
	beginResetModel();

	m_nRows = pSpec->rows;
	m_nCols = pSpec->cols;

	m_vecItems.clear();
	m_vecItems.resize(m_nRows * m_nCols);

	std::vector<tableCell> pp = pSpec->items;
	for (unsigned int i = 0, h = 0; i < pp.size(); i++, h++) {
		m_vecItems[h] = {QString::fromStdString(pp[i].text), pp[i].align};

		h += pp[i].colspan - 1;
	}

	endResetModel();
}

int TWTableModel::rowCount(const QModelIndex& parent) const
{
	return m_nRows-1;
}

int TWTableModel::columnCount(const QModelIndex& parent) const
{
	return m_nCols;
}

QVariant TWTableModel::GetData(int row, int col, int role) const
{
	int i = row*m_nCols + col;
	const ItemInfo& ii = m_vecItems[i];

	switch (role) {
		case Qt::DisplayRole:
			return ii.sText;

		case Qt::TextAlignmentRole:
			return ii.align;

		default:
			return QVariant();
	}
}

QVariant TWTableModel::data(const QModelIndex& index, int role) const
{
	return GetData(1+index.row(), index.column(), role);
}

QVariant TWTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal)
		return GetData(0, section, role);
	else
		return QVariant();
}
