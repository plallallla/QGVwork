#include "MyViewTest.h"

MyViewTest::MyViewTest(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//Message
	connect(ui.Viewer, &ROIView::PositionMessage, [=](QString msg) {
		ui.label_Msg->setText(msg);
	});
	connect(ui.Viewer, &ROIView::MeasureMessage, [=](QLineF l) {
		ui.label_Msg->setText(QStringLiteral("起点:%0  终点:%1  长度:%3  角度:%4")
			.arg(QString(("(%0,%1)")).arg((int)l.p1().x()).arg((int)l.p1().y()))
			.arg(QString(("(%0,%1)")).arg((int)l.p2().x()).arg((int)l.p2().y()))
			.arg((int)l.length()).arg(QString::number(l.angleTo(QLineF(QPointF(0, 0), QPointF(1, 0))), 'f', 1)));
	});
	//Open
	connect(ui.pushButton_Open, &QPushButton::pressed, [=]() {
		ui.Viewer->OpenPicture("test.png");
	});
	//Draw
	connect(ui.pushButton_Draw, &QPushButton::pressed, [=]() {
		if (ui.lineEdit->text().isEmpty())
		{
			ui.Viewer->DrawRect();
		}
		else
		{
			ui.Viewer->DrawRect(ui.lineEdit->text());
		}
	});
	//AffineRect
	connect(ui.pushButton_AffineRect, &QPushButton::pressed, [=]() {
		if (ui.lineEdit->text().isEmpty())
		{
			ui.Viewer->DrawAffineRect();
		}
		else
		{
			ui.Viewer->DrawRect(ui.lineEdit->text());
		}
	});
	//Description
	connect(ui.pushButton_Description, &QPushButton::pressed, [=]() {
		QVector<QPointF> dots = { QPointF(1,1),QPointF(100,10),QPointF(120,90),QPointF(90,120) };
		QStringList ans = { QString("point1"),QString("point2") ,QString("point3") ,QString("point4") };
		ui.Viewer->AddPolygonDescription(dots, ans);
	});
	//Clear
	connect(ui.pushButton_Clear, &QPushButton::pressed, [=]() {
		ui.Viewer->RemoveROI();
	});
	//UpdateMessage
	connect(ui.pushButton_UpdateMSG, &QPushButton::pressed, [=]() {
		ui.plainTextEdit->clear();
		UpdateMSG();
	});
	//ClearMessage
	connect(ui.pushButton_ClearMSG, &QPushButton::pressed, [=]() {
		ui.plainTextEdit->clear();
	});
	//ByIndex
	connect(ui.checkBox_ByName, &QCheckBox::stateChanged, [=](int t) {
		ui.lineEdit_Search->clear();
		if (t != 0)
		{
			SearchTagIndex = false;
			ui.lineEdit_Search->setPlaceholderText(QStringLiteral("输入目标图元对象名称"));
		}
		else
		{
			SearchTagIndex = true;
			ui.lineEdit_Search->setPlaceholderText(QStringLiteral("输入目标图元对象序列"));
		}
	});
	//Search
	connect(ui.pushButton_Search, &QPushButton::pressed, [=]() {
		auto p = ui.Viewer->AllItems();
		if (ui.checkBox_ByName->isChecked())
		{
			QString tagName = ui.lineEdit_Search->text();
			for (auto item : p)
			{
				if (tagName == item->data(ROIBASE_ITEM_NAME).toString() && item->data(ROIBASE_ITEM_INDEX).toInt() != -1)
				{
					ui.plainTextEdit->appendPlainText(QStringLiteral("找到了"));
					ui.plainTextEdit->appendPlainText(DescribeGraphicsItem(item));
				}
				break;
			}
		}
		else
		{
			int tagIndex = ui.lineEdit_Search->text().toInt();
			for (auto item : p)
			{
				if (tagIndex == item->data(ROIBASE_ITEM_INDEX).toInt())
				{
					ui.plainTextEdit->appendPlainText(QStringLiteral("找到了"));
					ui.plainTextEdit->appendPlainText(DescribeGraphicsItem(item));
				}
				break;
			}
		}
	});
	//Measure
	connect(ui.pushButton_Measure, &QPushButton::pressed, [=]() {
		ui.Viewer->Measure();
	});
	//Insert
	connect(ui.pushButton_Insert, &QPushButton::pressed, [=]() {
		switch (ui.comboBox_item->currentIndex())
		{
		case 0:
			ui.Viewer->AddQGVLine(QLineF(QPointF(0,0),QPointF(100,100)));
			break;
		case 1:
			ui.Viewer->AddQGVRect(QRectF(40,40,85,85));
			break;
		case 2:
			ui.Viewer->AddQGVEllipse(QRectF(40, 40, 85, 85));
			break;
		case 3:
		{
			QVector<QPointF> dot = { QPointF(1,1),QPointF(100,10),QPointF(120,90),QPointF(90,120) };
			ui.Viewer->AddQGVPolygon(dot);
			break;
		}
		case 4:
			ui.Viewer->AddQGVText(QStringLiteral("文字实例"));
			break;
		case 5:
		{
			QPixmap * pix = new QPixmap("aq.png");
			ui.Viewer->AddQGVPixmap(*pix);
			delete pix;
		}
		default:
			break;
		}
	});
}

MyViewTest::~MyViewTest()
{}

QString MyViewTest::DescribeGraphicsItem(QGraphicsItem *tag)
{
	QString ans;
	switch (tag->type())
	{
	case ROIBaseItem::AFFINERECT:
	{
		ROIAffineRect *AffineRect = static_cast<ROIAffineRect *>(tag);
		ans += QStringLiteral("作为一个矩形,他的特征为\n左x:%0,上y:%1,\n右x:%2,下y:%3\n")
			.arg(AffineRect->GetRectData()[0]).arg(AffineRect->GetRectData()[1]).
			arg(AffineRect->GetRectData()[2]).arg(AffineRect->GetRectData()[3]);
		AffineRect->GetRotateRectData();
		ans += QStringLiteral("作为一个旋转矩形,他的特征为\n中心x:%0,中心y:%1,\n水平边长:%2,斜边长:%3,旋转角:%4\n")
			.arg(AffineRect->GetRotateRectData()[0]).arg(AffineRect->GetRotateRectData()[1])
			.arg(AffineRect->GetRotateRectData()[2]).arg(AffineRect->GetRotateRectData()[3])
			.arg(AffineRect->GetRotateRectData()[4]);
		ans += QStringLiteral("作为一个仿射矩形,他的特征为\nP0X:%0,P0Y:%1,\nP1X:%2,P1Y:%3,\nP2X:%4,P2Y:%5,\n")
			.arg(AffineRect->GetAffineRectData()[0]).arg(AffineRect->GetAffineRectData()[1])
			.arg(AffineRect->GetAffineRectData()[2]).arg(AffineRect->GetAffineRectData()[3])
			.arg(AffineRect->GetAffineRectData()[4]).arg(AffineRect->GetAffineRectData()[5]);
		break;
	}
	default:
		ans = QStringLiteral("未知的类型");
	}
	return ans;
}

void MyViewTest::UpdateMSG()
{
	auto p = ui.Viewer->AllItems();
	if (p.size() == 0)
	{
		ui.plainTextEdit->appendPlainText(QStringLiteral("当前没有图元对象"));
		return;
	}
	for (auto item : p)
	{
		int index = item->data(ROIBASE_ITEM_INDEX).toInt();
		if (index == -1 || index == 0)
		{
			continue;
		}
		QString msg = QString("ID:%0,NAME:%1").arg(index).
			arg(item->data(ROIBASE_ITEM_NAME).toString());
		ui.plainTextEdit->appendPlainText(msg);
	}
}

