#pragma once
#include "ROIBaseItem.h"

class ROIVIEW_CLASS_EXPORT ROIAffineRect :
	public ROIBaseItem
{
public:
	//����������
	explicit ROIAffineRect();
	explicit ROIAffineRect(qreal v, qreal b, qreal angel);
	~ROIAffineRect();
	//���ܺ���
	void SetEnable(CHANGEMODE tag, bool able = false);
	//���غ���
	int type() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

	//��Ϊ���λ�ȡ��x,��y,��x,��y�ĸ�ֵ
	QVector<double> GetRectData();
	//��Ϊ��ת���λ�ȡ����x,����y,���,�߶�,�Ƕ�(�������,˳ʱ��Ϊ��,�Ƕ���)���ֵ
	QVector<double> GetRotateRectData();
	//��Ϊ������λ�ȡ���ϵ����ϵ����µ�����ֵ
	QVector<double> GetAffineRectData();
protected:
	//�¼���Ӧ����,�ƶ����α���������º���������
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
	//��������ֵ�����ڳ�ʼ������Ӧ�¼�
	void Reset(qreal v, qreal b, qreal angel);
	//����ʵ��Ԥ��Ч��
	void SetTempShow();
	QPolygonF MakePolygon(qreal v, qreal b, qreal angel) const;
	POSITION WhereIsPos(QPointF);
private:
	bool m_bRotate = true;
	bool m_bResize = true;
	bool m_bReangel = true;
	qreal m_dbVerticalLength;//ˮƽ�߱߳�
	qreal m_dbBevelLength;//б�߱߳�
	qreal m_dbAngle;//�нǽǶ�
	QPolygonF m_Polygon;//���ɶ����
	QGraphicsPolygonItem m_tempShow;//Ԥ��ͼ��
	QPointF m_ptReangel;//�нǸı�ê��
	QPointF m_ptRotate;//��ע��תê��
	QPointF m_ptResize;//��С�ı�ê��
	CHANGEMODE m_changemode;//�仯ģʽ
	QPointF m_ptClick;//�����¼��
	QPointF m_ptULStable;//���Ͻ��ȶ�������

	//��ת�Ż�
	QTransform m_RotateTransform;
	qreal m_angle = 0;
};
