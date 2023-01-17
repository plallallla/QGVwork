#pragma once
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QPainter>
#include <QGraphicsPolygonItem>
#include <QGraphicsScene>
#include <QVector2D>
#include <QVector3D>
#include <qDebug>

#if defined(ROIVIEW_EXPORT) 
#define ROIVIEW_CLASS_EXPORT __declspec(dllexport) 
#else 
#define ROIVIEW_CLASS_EXPORT __declspec(dllimport) 
#endif

#define ROIBASE_ITEM_INDEX 1
#define ROIBASE_ITEM_NAME 2

class ROIBaseItem :
	public QGraphicsItem
{
public:
	enum TYPE
	{

		BASE = 98,//基类
		AFFINERECT = 99,//仿射矩形
		QUADRI = 100,//四边形
	};
	enum POSITION
	{
		COMMON,
		UP_LEFT,
		UP_MID,
		UP_RIGHT,
		LEFT_MID,
		RIGHT_MID,
		DOWN_LEFT,
		DOWN_MID,
		DOWN_RIGHT,
	};
	enum CHANGEMODE
	{
		NONE,
		REANGEL,
		ROTATE,
		RESIZE,
	};
	QPainterPath shape() const;
	QRectF boundingRect() const override;
	virtual int type() const;
	void SetIndex(int index);
	int GetIndex() const;
	void SetName(const QString & name);
	QString GetName() const;
protected:
	bool Approach(QPointF p1, QPointF p2);
	QPainterPath m_shape;
	QRectF m_boundary;
};
