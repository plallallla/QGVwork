#pragma once
#include <QGraphicsView>
#include <qfiledialog.h>
#include <QPointF>
#include <QWheelEvent>
#include <qscrollbar.h>
#include <QDebug>
#include <QMessageBox>
#include <QBrush>

#include "ROIScene.h"
#include "ROIBaseItem.h"
#include "ROIAffineRect.h"
#include "items.h"

#if defined(ROIVIEW_EXPORT) 
#define ROIVIEW_CLASS_EXPORT __declspec(dllexport) 
#else 
#define ROIVIEW_CLASS_EXPORT __declspec(dllimport) 
#endif

enum QGVItem
{
	QGVLineItem,
	QGVRectItem,
	QGVEllipseItem,
	QGVPathItem,
	QGVPolygonItem,
	QGVSimpleTextItem,
	QGVPixMapItem,
};


//这是基于QGraphicsView派生的View类
//支持动态加载图片、删除图片、右键自适应、中键拖动
//支持加入半径可变的圆形、圆环、夹角边长可变的四边形，支持删除标注
//支持设置打开文件夹对话框的初始显示路径
class ROIVIEW_CLASS_EXPORT ROIView :
	public QGraphicsView
{

	Q_OBJECT

public:
	ROIView(QWidget * parent = nullptr);
	ROIView(ROIScene * scene, QWidget * parent = nullptr);
	~ROIView();

	/*打开图片,管理标注*/

	//从默认文件夹打开文件对话框以选择图片，打开图片，设置标注的最大边
	void OpenPicture();
	//打开文件路径为picture的图片，设置标注的最大边
	void OpenPicture(QString picture);
	//打开以QImage形式存储的图片，设置标注的最大边
	void OpenPicture(const QImage * picture);
	//移除已经载入的图片
	void RemovePicture();
	//移除选中的ROI标注，未选中则删除全部ROI
	void RemoveROI();
	//重置图片大小与位置
	void ResetPicture();
	//设置打开文件夹的默认路径
	void SetFolderPath(QString path);

	/*加入原生图形*/

	//加入线段
	void AddQGVLine(const QLineF &line, int index = -1, const QString &name = "QGVLine", QPointF pos = QPointF(),
		const QPen &pen = QPen(Qt::red, 2));
	//加入矩形
	void AddQGVRect(const QRectF &rect, int index = -1, const QString &name = "QGVRect", QPointF pos = QPointF(),
		const QPen &pen = QPen(Qt::red, 2), const QBrush &brush = QBrush(Qt::transparent));
	//加入椭圆
	void AddQGVEllipse(QRectF rect, int index = -1, const QString &name = "QGVEllipse", QPointF pos = QPointF(),
		const QPen &pen = QPen(Qt::red, 2), const QBrush &brush = QBrush(Qt::transparent));
	//加入多边形
	void AddQGVPolygon(const QVector<QPointF> &points, int index = -1, const QString &name = "QGVRect", QPointF pos = QPointF(),
		const QPen &pen = QPen(Qt::red, 2), const QBrush &brush = QBrush(Qt::transparent));
	//加入文字
	void AddQGVText(const QString text,  QPointF pos = QPointF(), const QString &name = "QGVRect", int index = -1,
		const QColor& color = Qt::red, const QFont &font = QFont("宋体", 10));
	//加入位图
	void AddQGVPixmap(const QPixmap &pixmap, QPointF pos = QPointF(), const QString &name = "QGVPixmap", int index = -1);
	
	/*动态绘制函数*/

	//测量距离
	void Measure();
	//动态绘制一个矩形
	void DrawRect(const QString &name = "RectItem", int index = -1);
	//动态绘制一个仿射矩形
	void DrawAffineRect(const QString &name = "AffineRectItem", int index = -1);

	/*文字描述功能*/

	//加入文字到画面中
	void AddText(const QString & str, const QFont & font = QFont("宋体", 10), QPointF defaultPosition = QPointF());
	//显示序号的QGraphicsTextItem实例，默认绿色
	void ShowTextIndex(const QString & text, const QFont & font = QFont("宋体", 10), const QColor& color = QColor(0, 255, 0), QPointF defaultPosition = QPointF(0, 0));
	//显示结果的QGraphicsTextItem实例，默认红色
	void ShowTextResult(const QString & text, const QFont & font = QFont("宋体", 10), const QColor& color = QColor(255, 0, 0), QPointF defaultPosition = QPointF(0, 20));

	/*图形直接加入函数*/

	//加入四边形到画面中，可设置水平边长、斜边边长、四边形夹角，不设置均为默认参数
	void AddAffineRect(const QString &name = "RectItem", int index = -1, qreal defaultVerticalLength = -1, qreal defaultBevelLength = -1, qreal defaultAngel = -1, QPointF defaultPosition = QPointF());
	//加入矩形到画面中，可设置宽、高、位置，不设置均为默认参数
	void AddRect(const QString &name = "RectItem", int index = -1, qreal defaultWidth = -1, qreal defaultHeight = -1, QPointF defaultPosition = QPointF());
	//加入多点连线图像描述到画面中
	void AddPolygonDescription(const QVector<QPointF> &points, const QStringList &description, int DotDiameter = 5,
		const QBrush & DotBrush = QBrush(Qt::green), const QPen & LinePen = QPen(Qt::darkYellow, 2, Qt::DotLine), const QFont & font = QFont("宋体", 10));
	/*功能函数*/

	//获取Scene指针
	ROIScene * GetROIScene();
	//返回实际显示图形区域
	QRectF GetShowRect();
	//确认是否拥有图片
	bool HasPicture();
	//设置越界颜色
	void SetOutColor(const QColor & color);
	//获取所有的绘图对象
	QList<QGraphicsItem*> AllItems();


	/*临时添加功能函数*/
	QRectF GetDrawRectRegion();
signals:
	void PositionMessage(QString);
	void MeasureMessage(QLineF);

protected:
	//以下为事件鼠标事件、滚轮事件响应函数
	//完成了缩放、平移、还原的功能
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
private:
	int m_uIndex = 0;
	int m_uIndextag;
	QString m_uName;
	QColor m_OutColor = Qt::gray;
	qreal m_dDefaultWidth = 150;
	qreal m_dDefaultHeight = 200;
	qreal m_dDefaultRadius = 80;
	qreal m_dDefaultBigRadius = 90;
	qreal m_dDefaultSmallRadius = 40;
	qreal m_dDafaultVerticalLength = 200;
	qreal m_dDafaultBevelLength = 200;
	qreal m_dDafaultAngel = 45;
	QString m_qstrFolderPath = "";
	ROIScene *m_ptrScene;
	QPointF m_pPressed;
	QGraphicsRectItem DrawedRect;
	QGraphicsLineItem MeasureLine;
	QGraphicsLineItem vAffineLine;
	QGraphicsLineItem bAffineLine;
	QGraphicsTextItem* m_textIndex;
	QGraphicsTextItem* m_textResult;
	bool isMousePressed = false;
	bool isMicro = false;
	bool Drag = false;
	bool Measuring = false;
	bool DrawingRect = false;
	bool vDrawingAffineRect = false;
	bool bDrawingAffineRect = false;
};

