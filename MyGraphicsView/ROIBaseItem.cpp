#include "ROIBaseItem.h"


QPainterPath ROIBaseItem::shape() const
{
	return m_shape;
}

QRectF ROIBaseItem::boundingRect() const
{
	return m_boundary;
}

int ROIBaseItem::type() const
{
	return ROIBaseItem::BASE;
}

void ROIBaseItem::SetIndex(int index)
{
	setData(ROIBASE_ITEM_INDEX,index);
}

int ROIBaseItem::GetIndex() const
{
	return data(ROIBASE_ITEM_INDEX).toInt();
}

void ROIBaseItem::SetName(const QString & name)
{
	setData(ROIBASE_ITEM_NAME, name);
}

QString ROIBaseItem::GetName() const
{
	return data(ROIBASE_ITEM_INDEX).toString();
}

bool ROIBaseItem::Approach(QPointF p1, QPointF p2)
{
	if ((p1 - p2).manhattanLength() <= 6)
	{
		return true;
	}
	return false;
}

