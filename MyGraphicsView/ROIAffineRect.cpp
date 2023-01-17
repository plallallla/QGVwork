#include "ROIAffineRect.h"

inline double ToRadian(double x)
{
	return 3.1415 / 180 * (x);
}

inline double ToAngle(double x)
{
	return 180 / 3.1415 * (x);
}

inline double ToAcute(double x)
{
	if (0 <= x && x <= 90)
	{
		return x;
	}
	else if (90 < x && x < 180)
	{
		return x - 180;
	}
	else if (180 <= x && x <= 270)
	{
		return x - 180;
	}
	else if (270 < x && x < 360)
	{
		return x - 360;
	}
}
ROIAffineRect::ROIAffineRect()
{
}

ROIAffineRect::ROIAffineRect(qreal v, qreal b, qreal angel)
{
	setAcceptHoverEvents(true);
	Reset(v, b, angel);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
}

ROIAffineRect::~ROIAffineRect()
{
}

void ROIAffineRect::SetEnable(CHANGEMODE tag, bool able)
{
	switch (tag)
	{
	case ROIAffineRect::ROTATE:
		m_bRotate = able;
		return;
	case ROIAffineRect::RESIZE:
		m_bResize = able;
		return;
	case ROIAffineRect::REANGEL:
		m_bReangel = able;
		return;
	}
}


void ROIAffineRect::Reset(qreal v, qreal b, qreal angel)
{
	//设置四边形属性
	m_dbVerticalLength = v;
	m_dbBevelLength = b;
	m_dbAngle = angel;
	m_Polygon.clear();
	m_Polygon = MakePolygon(v, b, angel);
	//设置画布为四边形外接矩形
	m_boundary = m_Polygon.boundingRect();
	m_boundary = QRectF(m_Polygon.boundingRect().x() - 10, m_Polygon.boundingRect().y() - 10,
		m_Polygon.boundingRect().width() + 20, m_Polygon.boundingRect().height() + 20);
	//设置点击判定范围为四边形周边
	QPainterPath big, small;
	//****修复方向绘制导致点击判定消失的bug
	double _v = abs(v);
	double _b = abs(b);
	//****不取绝对值会导致反向绘制时点击判定消失
	big.addPolygon(MakePolygon(_v + 10, _b + 10, angel));
	small.addPolygon(MakePolygon(_v - 10, _b - 10, angel));
	m_shape = big - small;
	//设置锚点
	qreal Rangel = ToRadian(m_dbAngle);
	//夹角改变锚点为上方平行边的中间点
	m_ptReangel = (QPointF(-v + b * cos(Rangel), -b * sin(Rangel)) / 2
		+ QPointF(v + b * cos(Rangel), -b * sin(Rangel)) / 2) / 2;
	//标注旋转锚点为左下方端点
	m_ptRotate = QPointF(-v - b * cos(Rangel), b * sin(Rangel)) / 2;
	//大小改变锚点为右下方端点
	m_ptResize = QPointF(v - b * cos(Rangel), b * sin(Rangel)) / 2;

}

void ROIAffineRect::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
	QGraphicsItem::hoverEnterEvent(event);
	if (!isSelected())
	{
		return;
	}
	switch (WhereIsPos(event->pos()))
	{
	case COMMON:
		setFlag(QGraphicsItem::ItemIsMovable, true);
		setToolTip(QStringLiteral("点击拖动以移动"));
		m_changemode = NONE;
		return;
	case UP_MID:
		setFlag(QGraphicsItem::ItemIsMovable, false);
		m_changemode = m_bReangel ? REANGEL : NONE;
		if (m_changemode != NONE)
		{
			setToolTip(QStringLiteral("点击拖动以改变夹角"));
		}
		return;
	case DOWN_LEFT:
		setFlag(QGraphicsItem::ItemIsMovable, false);
		m_changemode = m_bRotate ? ROTATE : NONE;
		if (m_changemode != NONE)
		{
			setToolTip(QStringLiteral("点击拖动以旋转"));
		}
		return;
	case UP_RIGHT:
		setFlag(QGraphicsItem::ItemIsMovable, false);
		m_changemode = m_bResize ? RESIZE : NONE;
		if (m_changemode != NONE)
		{
			setToolTip(QStringLiteral("点击拖动以改变长短(ctrl)"));
		}
		return;
	}
}

void ROIAffineRect::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mousePressEvent(event);
	SetTempShow();
	scene()->addItem(&m_tempShow);
	m_ptClick = event->scenePos();
	//左上角场景坐标
	m_ptULStable = mapToScene(QPointF(-m_dbVerticalLength + m_dbBevelLength * cos(ToRadian(m_dbAngle)),
		-m_dbBevelLength * sin(ToRadian(m_dbAngle))) / 2);
}

void ROIAffineRect::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	qreal gap;
	QLineF now, diagonal;
	switch (m_changemode)
	{
	case NONE:
		return;
	case REANGEL:
		gap = QLineF(QPointF(0, 0), mapFromScene(m_ptClick)).angleTo(QLineF(QPointF(0, 0), event->pos()));
		Reset(m_dbVerticalLength, m_dbBevelLength, m_dbAngle + gap);
		m_ptClick = event->scenePos();
		update();
		return;
	case ROTATE:
		now = QLineF(scenePos(), event->scenePos());
		diagonal = QLineF(scenePos(), scenePos() + m_ptRotate);
		setRotation(now.angleTo(diagonal));
		m_ptClick = event->scenePos();
		return;
	case RESIZE:
		if ((event->modifiers()&Qt::ControlModifier) != 0)//按下control作为修饰键
		{
			//对称式缩放
			qreal sita1 = QLineF(QPointF(0, 0), QPointF(1, 0)).angleTo(QLineF(QPointF(0, 0), event->pos()));
			qreal sita2 = m_dbAngle - sita1;
			qreal sita3 = 180 - m_dbAngle;
			qreal length = QLineF(QPointF(0, 0), event->pos()).length() * 2;
			qreal v = abs(sin(ToRadian(sita2))) / abs(sin(ToRadian(sita3)))*length;
			qreal b = abs(sin(ToRadian(sita1))) / abs(sin(ToRadian(sita3)))*length;
			Reset(v, b, m_dbAngle);
		}
		else
		{
			//非对称式缩放
			QPointF center = (m_ptULStable + event->scenePos()) / 2;
			qreal sita1 = QLineF(QPointF(0, 0), QPointF(1, 0)).angleTo(QLineF(QPointF(0, 0), event->pos()));
			qreal sita2 = m_dbAngle - sita1;
			qreal sita3 = 180 - m_dbAngle;
			qreal length = QLineF(center, event->scenePos()).length() * 2;
			qreal v = abs(sin(ToRadian(sita2))) / abs(sin(ToRadian(sita3)))*length;
			qreal b = abs(sin(ToRadian(sita1))) / abs(sin(ToRadian(sita3)))*length;
			Reset(v, b, m_dbAngle);
			setPos(center);
		}
		update();
		return;
	}
}

void ROIAffineRect::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	scene()->removeItem(&m_tempShow);
}


void ROIAffineRect::SetTempShow()
{
	m_tempShow.setPolygon(m_Polygon);
	m_tempShow.setZValue(-1);
	QPen pen(Qt::red);
	pen.setStyle(Qt::DotLine);
	m_tempShow.setPen(pen);
	m_tempShow.setBrush(Qt::transparent);
	m_tempShow.setRotation(rotation());
	m_tempShow.setPos(scenePos());
}

QPolygonF ROIAffineRect::MakePolygon(qreal v, qreal b, qreal angel) const
{
	QPolygonF ans;
	qreal Rangel = ToRadian(angel);
	ans << QPointF(v - b * cos(Rangel), b * sin(Rangel)) / 2//右下角
		<< QPointF(-v - b * cos(Rangel), b * sin(Rangel)) / 2//左下角
		<< QPointF(-v + b * cos(Rangel), -b * sin(Rangel)) / 2//左上角
		<< QPointF(v + b * cos(Rangel), -b * sin(Rangel)) / 2;//右上角
	return ans;
}



ROIBaseItem::POSITION ROIAffineRect::WhereIsPos(QPointF pos)
{
	if (Approach(pos, m_ptReangel))
	{
		return UP_MID;
	}
	else if (Approach(pos, m_ptRotate))
	{
		return DOWN_LEFT;
	}
	else if (Approach(pos, m_ptResize))
	{
		return UP_RIGHT;
	}
	return COMMON;
}


int ROIAffineRect::type() const
{
	return ROIBaseItem::AFFINERECT;
}

void ROIAffineRect::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(widget);
	painter->setBrush(Qt::transparent);
	painter->setRenderHint(QPainter::SmoothPixmapTransform);
	if (option->state & QStyle::State_Selected)
	{
		//选中状态画法
		painter->setPen(QPen(Qt::green, 2));
		painter->drawPolygon(m_Polygon);
		painter->setPen(Qt::black);
		painter->setBrush(Qt::transparent);
		//锚点绘制准备
		QPainterPath path, verpath(QPointF(2, 2)), horpath(QPointF(-2, 2));
		verpath.lineTo(-2, -2);
		horpath.lineTo(2, -2);
		path.addPath(verpath);
		path.addPath(horpath);
		//夹角改变锚点绘制
		painter->translate(m_ptReangel);
		if (m_bReangel)
		{
			painter->drawPath(path);
		}
		//旋转锚点绘制
		painter->translate(m_ptRotate - m_ptReangel);
		if (m_bRotate)
		{
			painter->drawPath(path);
		}
		//大小改变锚点绘制
		painter->translate(m_ptResize - m_ptRotate);
		if (m_bResize)
		{
			painter->drawPath(path);
		}
	}
	else
	{
		//未选中状态画法
		painter->setPen(QPen(Qt::red, 2));
		painter->drawPolygon(m_Polygon);
	}
}

QVector<double> ROIAffineRect::GetRectData()
{
	QVector<double> ans;
	if (m_Polygon[0].x() < 0)
	{
		QPointF topLeft = mapToScene(-m_Polygon[2]);
		QPointF topRight = mapToScene(-m_Polygon[3]);
		QPointF bottomRight = mapToScene(-m_Polygon[0]);
		ans.push_back(topLeft.x());
		ans.push_back(topLeft.y());
		ans.push_back(topRight.x());
		ans.push_back(bottomRight.y());
		return ans;
	}
	QPointF topLeft = mapToScene(m_Polygon[2]);
	QPointF topRight = mapToScene(m_Polygon[3]);
	QPointF bottomRight = mapToScene(m_Polygon[0]);
	ans.push_back(topLeft.x());
	ans.push_back(topLeft.y());
	ans.push_back(topRight.x());
	ans.push_back(bottomRight.y());
	return ans;
}

QVector<double> ROIAffineRect::GetRotateRectData()
{
	QVector<double> ans;
	QPointF Center = mapToScene((m_Polygon[0] + m_Polygon[2]) / 2);
	QLineF v = QLineF(mapToScene(m_Polygon[0]), mapToScene(m_Polygon[1]));
	QLineF b = QLineF(m_Polygon[0], m_Polygon[3]);
	ans.push_back(Center.x());
	ans.push_back(Center.y());
	qreal angel;
	qreal rot = rotation();
	if ((0 <= rot && rot < 90) || (180 <= rot && rot < 270))
	{
		ans.append(v.length());
		ans.append(b.length());
	}
	else if((90 <= rot && rot < 180) || (270 <= rot && rot < 360))
	{
		ans.append(b.length());
		ans.append(v.length());
	}
	rot = ToAcute(rot);
	ans.append(rot);//角度有待验证
	return ans;
}

QVector<double> ROIAffineRect::GetAffineRectData()
{
	QVector<double> ans;
	qreal rot = rotation();
	QPointF topLeft = mapToScene(m_Polygon[1]);//P0
	QPointF topRight = mapToScene(m_Polygon[0]);//P1
	QPointF bottomRight = mapToScene(m_Polygon[2]);//P2
	ans.push_back(topLeft.x());
	ans.push_back(topLeft.y());
	ans.push_back(topRight.x());
	ans.push_back(topRight.y());
	ans.push_back(bottomRight.x());
	ans.push_back(bottomRight.y());
	return ans;
}
