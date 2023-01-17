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


//���ǻ���QGraphicsView������View��
//֧�ֶ�̬����ͼƬ��ɾ��ͼƬ���Ҽ�����Ӧ���м��϶�
//֧�ּ���뾶�ɱ��Բ�Ρ�Բ�����нǱ߳��ɱ���ı��Σ�֧��ɾ����ע
//֧�����ô��ļ��жԻ���ĳ�ʼ��ʾ·��
class ROIVIEW_CLASS_EXPORT ROIView :
	public QGraphicsView
{

	Q_OBJECT

public:
	ROIView(QWidget * parent = nullptr);
	ROIView(ROIScene * scene, QWidget * parent = nullptr);
	~ROIView();

	/*��ͼƬ,�����ע*/

	//��Ĭ���ļ��д��ļ��Ի�����ѡ��ͼƬ����ͼƬ�����ñ�ע������
	void OpenPicture();
	//���ļ�·��Ϊpicture��ͼƬ�����ñ�ע������
	void OpenPicture(QString picture);
	//����QImage��ʽ�洢��ͼƬ�����ñ�ע������
	void OpenPicture(const QImage * picture);
	//�Ƴ��Ѿ������ͼƬ
	void RemovePicture();
	//�Ƴ�ѡ�е�ROI��ע��δѡ����ɾ��ȫ��ROI
	void RemoveROI();
	//����ͼƬ��С��λ��
	void ResetPicture();
	//���ô��ļ��е�Ĭ��·��
	void SetFolderPath(QString path);

	/*����ԭ��ͼ��*/

	//�����߶�
	void AddQGVLine(const QLineF &line, int index = -1, const QString &name = "QGVLine", QPointF pos = QPointF(),
		const QPen &pen = QPen(Qt::red, 2));
	//�������
	void AddQGVRect(const QRectF &rect, int index = -1, const QString &name = "QGVRect", QPointF pos = QPointF(),
		const QPen &pen = QPen(Qt::red, 2), const QBrush &brush = QBrush(Qt::transparent));
	//������Բ
	void AddQGVEllipse(QRectF rect, int index = -1, const QString &name = "QGVEllipse", QPointF pos = QPointF(),
		const QPen &pen = QPen(Qt::red, 2), const QBrush &brush = QBrush(Qt::transparent));
	//��������
	void AddQGVPolygon(const QVector<QPointF> &points, int index = -1, const QString &name = "QGVRect", QPointF pos = QPointF(),
		const QPen &pen = QPen(Qt::red, 2), const QBrush &brush = QBrush(Qt::transparent));
	//��������
	void AddQGVText(const QString text,  QPointF pos = QPointF(), const QString &name = "QGVRect", int index = -1,
		const QColor& color = Qt::red, const QFont &font = QFont("����", 10));
	//����λͼ
	void AddQGVPixmap(const QPixmap &pixmap, QPointF pos = QPointF(), const QString &name = "QGVPixmap", int index = -1);
	
	/*��̬���ƺ���*/

	//��������
	void Measure();
	//��̬����һ������
	void DrawRect(const QString &name = "RectItem", int index = -1);
	//��̬����һ���������
	void DrawAffineRect(const QString &name = "AffineRectItem", int index = -1);

	/*������������*/

	//�������ֵ�������
	void AddText(const QString & str, const QFont & font = QFont("����", 10), QPointF defaultPosition = QPointF());
	//��ʾ��ŵ�QGraphicsTextItemʵ����Ĭ����ɫ
	void ShowTextIndex(const QString & text, const QFont & font = QFont("����", 10), const QColor& color = QColor(0, 255, 0), QPointF defaultPosition = QPointF(0, 0));
	//��ʾ�����QGraphicsTextItemʵ����Ĭ�Ϻ�ɫ
	void ShowTextResult(const QString & text, const QFont & font = QFont("����", 10), const QColor& color = QColor(255, 0, 0), QPointF defaultPosition = QPointF(0, 20));

	/*ͼ��ֱ�Ӽ��뺯��*/

	//�����ı��ε������У�������ˮƽ�߳���б�߱߳����ı��μнǣ������þ�ΪĬ�ϲ���
	void AddAffineRect(const QString &name = "RectItem", int index = -1, qreal defaultVerticalLength = -1, qreal defaultBevelLength = -1, qreal defaultAngel = -1, QPointF defaultPosition = QPointF());
	//������ε������У������ÿ��ߡ�λ�ã������þ�ΪĬ�ϲ���
	void AddRect(const QString &name = "RectItem", int index = -1, qreal defaultWidth = -1, qreal defaultHeight = -1, QPointF defaultPosition = QPointF());
	//����������ͼ��������������
	void AddPolygonDescription(const QVector<QPointF> &points, const QStringList &description, int DotDiameter = 5,
		const QBrush & DotBrush = QBrush(Qt::green), const QPen & LinePen = QPen(Qt::darkYellow, 2, Qt::DotLine), const QFont & font = QFont("����", 10));
	/*���ܺ���*/

	//��ȡSceneָ��
	ROIScene * GetROIScene();
	//����ʵ����ʾͼ������
	QRectF GetShowRect();
	//ȷ���Ƿ�ӵ��ͼƬ
	bool HasPicture();
	//����Խ����ɫ
	void SetOutColor(const QColor & color);
	//��ȡ���еĻ�ͼ����
	QList<QGraphicsItem*> AllItems();


	/*��ʱ��ӹ��ܺ���*/
	QRectF GetDrawRectRegion();
signals:
	void PositionMessage(QString);
	void MeasureMessage(QLineF);

protected:
	//����Ϊ�¼�����¼��������¼���Ӧ����
	//��������š�ƽ�ơ���ԭ�Ĺ���
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

