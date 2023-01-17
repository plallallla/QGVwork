#include "ROIView.h"

ROIView::ROIView(QWidget* parent) : QGraphicsView(parent)
{
	m_ptrScene = new ROIScene();
	MeasureLine.setPen(QPen(Qt::red, 2));
	DrawedRect.setPen(QPen(Qt::red));
	vAffineLine.setPen(QPen(Qt::red));
	bAffineLine.setPen(QPen(Qt::red));
	setScene(m_ptrScene);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setResizeAnchor(QGraphicsView::AnchorUnderMouse);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);//强制全部刷新
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏水平条
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏水平条
	setStyleSheet(QString("#%0{background-color: rgb(%1,%2,%3);}").arg(objectName())
		.arg(m_OutColor.red()).arg(m_OutColor.green()).arg(m_OutColor.blue()));
	m_textIndex = new QGraphicsTextItem("");
	m_textResult = new QGraphicsTextItem("");
	m_textIndex->setData(ROIBASE_ITEM_INDEX, -1);
	m_textResult->setData(ROIBASE_ITEM_INDEX,-1);
	m_ptrScene->addItem(m_textIndex);
	m_ptrScene->addItem(m_textResult);
}

ROIView::ROIView(ROIScene * scene, QWidget * parent)
{
	m_ptrScene = scene;
	setScene(m_ptrScene);
	MeasureLine.setPen(QPen(Qt::red, 2));
	DrawedRect.setPen(QPen(Qt::red));
	vAffineLine.setPen(QPen(Qt::red));
	bAffineLine.setPen(QPen(Qt::red));

	isMicro = true;
	setResizeAnchor(QGraphicsView::AnchorUnderMouse);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏水平条
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏水平条
	setStyleSheet(QString("#%0{background-color: rgb(%1,%2,%3);}").arg(objectName())
		.arg(m_OutColor.red()).arg(m_OutColor.green()).arg(m_OutColor.blue()));
	m_textIndex = new QGraphicsTextItem("");
	m_textResult = new QGraphicsTextItem("");
	m_textIndex->setData(ROIBASE_ITEM_INDEX, -1);
	m_textResult->setData(ROIBASE_ITEM_INDEX, -1);
	m_ptrScene->addItem(m_textIndex);
	m_ptrScene->addItem(m_textResult);
}


ROIView::~ROIView()
{

}

void ROIView::OpenPicture()
{
	QString fileName = QFileDialog::getOpenFileName(
		this, "open image file",
		m_qstrFolderPath,
		"Image files (*.bmp *.jpg *.jpeg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
	if (fileName.isNull() || !m_ptrScene->SetBackground(fileName))
	{
		QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("打开图片文件失败！%1").arg(fileName));
		return;
	}
	ResetPicture();
	BaseItem::SetMaxHeight(m_ptrScene->GetShowRect().height());
	BaseItem::SetMaxWidth(m_ptrScene->GetShowRect().width());
}

void ROIView::OpenPicture(QString picture)
{
	if (!m_ptrScene->SetBackground(picture))
	{
		QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("打开图片文件失败！%1").arg(picture));
		return;
	}
	ResetPicture();
	BaseItem::SetMaxHeight(m_ptrScene->GetShowRect().height());
	BaseItem::SetMaxWidth(m_ptrScene->GetShowRect().width());
}

void ROIView::OpenPicture(const QImage * picture)
{
	m_ptrScene->SetBackground(picture);
	ResetPicture();
	BaseItem::SetMaxHeight(m_ptrScene->GetShowRect().height());
	BaseItem::SetMaxWidth(m_ptrScene->GetShowRect().width());
}

void ROIView::RemovePicture()
{
	m_ptrScene->removeBackGround();
}

void ROIView::RemoveROI()
{
	//if (!m_ptrScene->selectedItems().isEmpty())
	//{
	//	for (QGraphicsItem *i : m_ptrScene->selectedItems())
	//	{
	//		m_ptrScene->removeItem(i);
	//	}
	//}
	//else
	//{
	//	for (QGraphicsItem *i : m_ptrScene->items())
	//	{
	//		m_ptrScene->removeItem(i);
	//	}
	//}
	for (QGraphicsItem *i : m_ptrScene->items())
	{
		m_ptrScene->removeItem(i);
	}
}

void ROIView::ResetPicture()
{
	if (m_ptrScene->BackGroundIsNull())
	{
		//QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("未打开图片文件！"));
		return;
	}
	QRectF b = m_ptrScene->GetShowRect();
	QRectF a = QRectF(b.x() - b.width(), b.y() - b.height(), 3 * b.width(), 3 * b.height());
	setSceneRect(a);
	fitInView(m_ptrScene->GetShowRect(), Qt::KeepAspectRatio);
}

void ROIView::SetFolderPath(QString path)
{
	m_qstrFolderPath = path;
}

void ROIView::AddQGVLine(const QLineF & line, int index, const QString & name, QPointF pos,   const QPen & pen)
{
	QGraphicsLineItem *i = new QGraphicsLineItem(line);
	i->setData(ROIBASE_ITEM_INDEX, (index == -1 ? ++m_uIndex : index));
	i->setData(ROIBASE_ITEM_NAME, name);
	i->setPen(pen);
	m_ptrScene->addItem(i);
	if (!pos.isNull())
	{
		i->setPos(pos);
	}
}

void ROIView::AddQGVRect(const QRectF & rect, int index, const QString & name, QPointF pos, const QPen & pen, const QBrush & brush)
{
	QGraphicsRectItem *i = new QGraphicsRectItem(rect);
	i->setData(ROIBASE_ITEM_INDEX, (index == -1 ? ++m_uIndex : index));
	i->setData(ROIBASE_ITEM_NAME, name);
	i->setPen(pen);
	i->setBrush(brush);
	m_ptrScene->addItem(i);
	if (!pos.isNull())
	{
		i->setPos(pos);
	}
}

void ROIView::AddQGVEllipse(QRectF rect, int index, const QString & name, QPointF pos, const QPen & pen, const QBrush & brush)
{
	QGraphicsEllipseItem *i = new QGraphicsEllipseItem(rect);
	i->setData(ROIBASE_ITEM_INDEX, (index == -1 ? ++m_uIndex : index));
	i->setData(ROIBASE_ITEM_NAME, name);
	i->setPen(pen);
	i->setBrush(brush);
	m_ptrScene->addItem(i);
	if (!pos.isNull())
	{
		i->setPos(pos);
	}
}

void ROIView::AddQGVPolygon(const QVector<QPointF>& points, int index, const QString & name, QPointF pos, const QPen & pen, const QBrush & brush)
{
	QGraphicsPolygonItem *i = new QGraphicsPolygonItem(points);
	i->setData(ROIBASE_ITEM_INDEX, (index == -1 ? ++m_uIndex : index));
	i->setData(ROIBASE_ITEM_NAME, name);
	i->setPen(pen);
	i->setBrush(brush);
	m_ptrScene->addItem(i);
	if (!pos.isNull())
	{
		i->setPos(pos);
	}
}

void ROIView::AddQGVText(const QString text, QPointF pos, const QString & name, int index, const QColor & color, const QFont & font)
{
	QGraphicsTextItem *i = new QGraphicsTextItem(text);
	i->setData(ROIBASE_ITEM_INDEX, (index == -1 ? ++m_uIndex : index));
	i->setData(ROIBASE_ITEM_NAME, name);
	i->setFont(font);
	i->setDefaultTextColor(color);
	m_ptrScene->addItem(i);
	if (!pos.isNull())
	{
		i->setPos(pos);
	}
}

void ROIView::AddQGVPixmap(const QPixmap &pixmap, QPointF pos, const QString & name, int index)
{
	QGraphicsPixmapItem *i = new QGraphicsPixmapItem(pixmap);
	i->setData(ROIBASE_ITEM_INDEX, (index == -1 ? ++m_uIndex : index));
	i->setData(ROIBASE_ITEM_NAME, name);
	m_ptrScene->addItem(i);
	if (!pos.isNull())
	{
		i->setPos(pos);
	}
}


void ROIView::Measure()
{
	if (m_ptrScene->BackGroundIsNull())
	{
		//QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("未打开图片文件！"));
		return;
	}
	Measuring = true;
	DrawingRect = false;
	vDrawingAffineRect = false;
}

void ROIView::DrawRect(const QString &name, int index)
{
	if (m_ptrScene->BackGroundIsNull())
	{
		//QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("未打开图片文件！"));
		return;
	}
	Measuring = false;
	DrawingRect = true;
	vDrawingAffineRect = false;
	m_uIndextag = index == -1 ? ++m_uIndex : index;
	m_uName = name;
}

void ROIView::DrawAffineRect(const QString &name, int index)
{
	if (m_ptrScene->BackGroundIsNull())
	{
		//QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("未打开图片文件！"));
		return;
	}
	Measuring = false;
	DrawingRect = false;
	vDrawingAffineRect = true;
	m_uIndextag = index == -1 ? ++m_uIndex : index;
	m_uName = name;
}

void ROIView::AddText(const QString & str, const QFont & font, QPointF defaultPosition)
{
	if (m_ptrScene->BackGroundIsNull())
	{
		//QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("未打开图片文件！"));
		return;
	}
	QGraphicsTextItem * i = new QGraphicsTextItem(str);
	i->setFont(font);
	if (defaultPosition.isNull())
	{
		i->setPos(m_ptrScene->GetShowRect().center());
	}
	else
	{
		i->setPos(defaultPosition);
	}
	m_ptrScene->addItem(i);
}

void ROIView::ShowTextIndex(const QString & text, const QFont & font, const QColor& color, QPointF defaultPosition)
{
	if (m_ptrScene->BackGroundIsNull())
	{
		//QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("未打开图片文件！"));
		return;
	}
	//m_textIndex->setPlainText(text);
	m_textIndex->setFont(font);
	m_textIndex->setDefaultTextColor(color);
	m_textIndex->setPos(defaultPosition);
	m_textIndex->setHtml(QString("<div style='background:rgba(255, 255, 255, 100%);'>" + text + QString("</div>")));
}

void ROIView::ShowTextResult(const QString & text, const QFont & font, const QColor& color, QPointF defaultPosition)
{
	if (m_ptrScene->BackGroundIsNull())
	{
		//QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("未打开图片文件！"));
		return;
	}
	//m_textResult->setPlainText(text);
	m_textResult->setFont(font);
	m_textResult->setDefaultTextColor(color);
	m_textResult->setPos(defaultPosition);
	m_textResult->setHtml(QString("<div style='background:rgba(255, 255, 255, 100%);'>" + text + QString("</div>")));
}

void ROIView::AddAffineRect(const QString & name, int index, qreal defaultVerticalLength, qreal defaultBevelLength, qreal defaultAngel, QPointF defaultPosition)
{
	if (m_ptrScene->BackGroundIsNull())
	{
		//QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("未打开图片文件！"));
		return;
	}
	ROIAffineRect *i;
	if (defaultVerticalLength == -1 || defaultVerticalLength == -1)
	{
		i = new ROIAffineRect(m_dDefaultWidth, m_dDefaultHeight, m_dDafaultAngel);
	}
	else
	{
		i = new ROIAffineRect(defaultVerticalLength, defaultBevelLength, defaultAngel);
	}
	if (defaultPosition.isNull())
	{
		i->setPos(m_ptrScene->GetShowRect().center());
	}
	else
	{
		i->setPos(defaultPosition);
	}
	i->SetIndex(index == -1 ? ++m_uIndex : index);
	i->SetName(name);
	m_uName.clear();
	m_ptrScene->addItem(i);
}

void ROIView::AddRect(const QString & name, int index, qreal defaultWidth, qreal defaultHeight, QPointF defaultPosition)
{
	if (m_ptrScene->BackGroundIsNull())
	{
		//QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("未打开图片文件！"));
		return;
	}
	ROIAffineRect *i;
	if (defaultWidth == -1 || defaultHeight == -1)
	{
		i = new ROIAffineRect(m_dDefaultWidth, m_dDefaultHeight, 90);
	}
	else
	{
		i = new ROIAffineRect(defaultWidth, defaultHeight, 90);
	}
	if (defaultPosition.isNull())
	{
		i->setPos(m_ptrScene->GetShowRect().center());
	}
	else
	{
		i->setPos(defaultPosition);
	}
	i->SetIndex(index == -1 ? ++m_uIndex : index);
	i->SetName(name);
	m_uName.clear();
	m_ptrScene->addItem(i);
}

void ROIView::AddPolygonDescription(const QVector<QPointF> & points, const QStringList & description, int DotDiameter, const QBrush & DotBrush, const QPen & LinePen, const QFont & font)
{
	if (points.size() != description.size())
	{
		qDebug() << "Error:PolygonDescription dont match!";
		return;
	}
	int num = points.size();
	int radius = (DotDiameter - 1) / 2;
	for (int i = 0; i < num; i++)
	{
		QPointF p = points[i];
		QGraphicsEllipseItem *point = new QGraphicsEllipseItem(QRectF(p.x() - radius, p.y() - radius, DotDiameter, DotDiameter));
		point->setBrush(DotBrush);
		point->setPen(QPen(Qt::transparent));
		m_ptrScene->addItem(point);
		QGraphicsTextItem *des = new QGraphicsTextItem(description[i]);
		des->setDefaultTextColor(DotBrush.color());
		des->setFont(font);
		m_ptrScene->addItem(des);
		des->setPos(points[i].x(), points[i].y());
	}
	QGraphicsPolygonItem *line = new QGraphicsPolygonItem(points);
	line->setBrush(Qt::transparent);
	line->setPen(LinePen);
	line->setZValue(-1);
	m_ptrScene->addItem(line);
}

ROIScene * ROIView::GetROIScene()
{
	return m_ptrScene;
}

//返回实际显示图形区域
QRectF ROIView::GetShowRect()
{
	return m_ptrScene->GetShowRect();
}

bool ROIView::HasPicture()
{
	return !m_ptrScene->BackGroundIsNull();
}

void ROIView::SetOutColor(const QColor & color)
{
	m_OutColor = color;
	setStyleSheet(QString("#%0{background-color: rgb(%1,%2,%3);}").arg(objectName())
		.arg(m_OutColor.red()).arg(m_OutColor.green()).arg(m_OutColor.blue()));
}

void ROIView::wheelEvent(QWheelEvent * event)
{
	if (isMicro)
	{
		return;
	}
	if ((event->delta() > 0) && (this->matrix().m11() >= 10))
	{
		return;
	}
	else if ((event->delta() < 0) && (this->matrix().m11() <= 0.01))
	{
		return;
	}
	else
	{
		int wheelDeltaValue = event->delta();
		if (wheelDeltaValue > 0)
		{
			scale(1.2, 1.2);
		}
		else
		{
			scale(1.0 / 1.2, 1.0 / 1.2);
		}
	}
}

void ROIView::mousePressEvent(QMouseEvent * event)
{
	if (isMicro)
	{
		return;
	}
	//中键点击拖动移动画面
	m_pPressed = mapToScene(event->pos());
	if (event->button() == Qt::MidButton)
	{
		QGraphicsView::mousePressEvent(event);
		if (m_ptrScene->selectedItems().length() == 0)
		{
			isMousePressed = true;
			Drag = true;
		}
	}
	//右键单击自适应画面
	else if (event->button() == Qt::RightButton)
	{
		ResetPicture();
	}
	//测量模式下左键划线测量
	else if (event->button() == Qt::LeftButton && Measuring)
	{
		MeasureLine.setLine(QLineF(mapToScene(event->pos()), mapToScene(event->pos())));
		m_ptrScene->addItem(&MeasureLine);
		isMousePressed = true;
	}
	//绘制模式下左键左键绘制矩形
	else if (event->button() == Qt::LeftButton && DrawingRect)
	{

		DrawedRect.setRect(QRectF(m_pPressed, m_pPressed));
		m_ptrScene->addItem(&DrawedRect);
		isMousePressed = true;
	}
	//绘制模式下左键左键绘制仿射矩形的水平边
	else if (event->button() == Qt::LeftButton && vDrawingAffineRect)
	{

		vAffineLine.setLine(QLineF(m_pPressed, m_pPressed));
		m_ptrScene->addItem(&vAffineLine);
		isMousePressed = true;
	}
	else if (event->button() == Qt::LeftButton && bDrawingAffineRect)
	{

		bAffineLine.setLine(QLineF(vAffineLine.line().p2(), m_pPressed));
		m_ptrScene->addItem(&bAffineLine);
		isMousePressed = true;
	}
	QGraphicsView::mousePressEvent(event);
}

void ROIView::mouseMoveEvent(QMouseEvent * event)
{
	if (isMicro)
	{
		return;
	}
	QPoint scenePos = QPoint(mapToScene(event->pos()).x(), mapToScene(event->pos()).y());
	if (Drag && isMousePressed)
	{
		QPointF gap = 0.1*(mapToScene(event->pos()) - m_pPressed);
		verticalScrollBar()->setValue(-gap.y() + verticalScrollBar()->value());
		horizontalScrollBar()->setValue(-gap.x() + horizontalScrollBar()->value());
	}
	//范围限制
	if (!m_ptrScene->BackGroundIsNull() && scenePos.x() >= 0 && scenePos.y() >= 0
		&& scenePos.x() <= m_ptrScene->GetShowRect().width()
		&& scenePos.y() <= m_ptrScene->GetShowRect().height())
	{
		if (Measuring && isMousePressed)
		{
			MeasureLine.setLine(QLineF(MeasureLine.line().p1(), mapToScene(event->pos())));
			emit MeasureMessage(MeasureLine.line());
			return;
		}
		else if (DrawingRect && isMousePressed)
		{
			DrawedRect.setRect(QRectF(m_pPressed, mapToScene(event->pos())));
			return;
		}
		else if (vDrawingAffineRect && isMousePressed)
		{
			vAffineLine.setLine(QLineF(vAffineLine.line().p1(), mapToScene(event->pos())));
			return;
		}
		else if (bDrawingAffineRect && isMousePressed)
		{
			bAffineLine.setLine(QLineF(bAffineLine.line().p1(), mapToScene(event->pos())));
			return;
		}
		QImage temp = m_ptrScene->GetBackGround();
		QRgb RGBvalue = temp.pixel(scenePos);
		QString msg = QString("Position:(%0,%1)").arg(scenePos.x()).arg(scenePos.y());
		msg += QString("    RGB:(%0,%1,%2)").arg(qRed(RGBvalue)).arg(qGreen(RGBvalue)).arg(qBlue(RGBvalue));
		emit PositionMessage(msg);
	}

	QGraphicsView::mouseMoveEvent(event);
}

void ROIView::mouseReleaseEvent(QMouseEvent * event)
{
	if (isMicro)
	{
		return;
	}
	if (Measuring)
	{
		m_ptrScene->removeItem(&MeasureLine);
		Measuring = false;
	}
	else if (DrawingRect)
	{
		qreal w = DrawedRect.rect().width();
		qreal h = DrawedRect.rect().height();
		ROIAffineRect *i = new ROIAffineRect(w, h, 90);
		i->setPos((DrawedRect.rect().topLeft() + DrawedRect.rect().bottomRight()) / 2);
		i->SetEnable(ROIBaseItem::REANGEL, false);
		m_ptrScene->removeItem(&DrawedRect);
		m_ptrScene->addItem(i);
		i->SetIndex(m_uIndextag);
		i->SetName(m_uName);
		m_uName.clear();
		DrawingRect = false;
	}
	else if (vDrawingAffineRect)
	{
		vDrawingAffineRect = false;
		bDrawingAffineRect = true;
	}
	else if (bDrawingAffineRect)
	{
		QLineF vLine = vAffineLine.line();
		QLineF bLine = bAffineLine.line();
		double w = vLine.length();
		double h = bLine.length();
		double angel = vLine.angleTo(bLine);
		ROIAffineRect *i = new ROIAffineRect(w, h, angel);
		m_ptrScene->removeItem(&vAffineLine);
		m_ptrScene->removeItem(&bAffineLine);
		double rotation = vLine.angleTo(QLineF(QPointF(0, 0), QPointF(1, 0)));
		i->setRotation(rotation);
		m_ptrScene->addItem(i);
		i->SetIndex(m_uIndextag);
		i->SetName(m_uName);
		m_uName.clear();
		i->setPos((vLine.p1() + bLine.p2()) / 2);
		bDrawingAffineRect = false;
	}
	isMousePressed = false;
	if (Drag)
	{
		Drag = false;
	}
	QGraphicsView::mouseReleaseEvent(event);
}

void ROIView::mouseDoubleClickEvent(QMouseEvent * event)
{
	if (isMicro)
	{
		return;
	}
	resetTransform();
	QGraphicsView::mouseDoubleClickEvent(event);
}

QList<QGraphicsItem*> ROIView::AllItems()
{
	return m_ptrScene->items();
}

QRectF ROIView::GetDrawRectRegion()
{
	return DrawedRect.rect();
}


