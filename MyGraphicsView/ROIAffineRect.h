#pragma once
#include "ROIBaseItem.h"

class ROIVIEW_CLASS_EXPORT ROIAffineRect :
	public ROIBaseItem
{
public:
	//构造与析构
	explicit ROIAffineRect();
	explicit ROIAffineRect(qreal v, qreal b, qreal angel);
	~ROIAffineRect();
	//功能函数
	void SetEnable(CHANGEMODE tag, bool able = false);
	//重载函数
	int type() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

	//作为矩形获取左x,上y,右x,下y四个值
	QVector<double> GetRectData();
	//作为旋转矩形获取中心x,中心y,宽度,高度,角度(正负锐角,顺时针为正,角度制)五个值
	QVector<double> GetRotateRectData();
	//作为仿射矩形获取左上点右上点左下点六个值
	QVector<double> GetAffineRectData();
protected:
	//事件响应函数,移动、形变均基于以下函数的联动
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
	//设置特征值，用于初始化与响应事件
	void Reset(qreal v, qreal b, qreal angel);
	//用于实现预览效果
	void SetTempShow();
	QPolygonF MakePolygon(qreal v, qreal b, qreal angel) const;
	POSITION WhereIsPos(QPointF);
private:
	bool m_bRotate = true;
	bool m_bResize = true;
	bool m_bReangel = true;
	qreal m_dbVerticalLength;//水平边边长
	qreal m_dbBevelLength;//斜边边长
	qreal m_dbAngle;//夹角角度
	QPolygonF m_Polygon;//生成多边形
	QGraphicsPolygonItem m_tempShow;//预览图形
	QPointF m_ptReangel;//夹角改变锚点
	QPointF m_ptRotate;//标注旋转锚点
	QPointF m_ptResize;//大小改变锚点
	CHANGEMODE m_changemode;//变化模式
	QPointF m_ptClick;//点击记录点
	QPointF m_ptULStable;//左上角稳定点坐标

	//旋转优化
	QTransform m_RotateTransform;
	qreal m_angle = 0;
};
