#pragma once

#include <QtWidgets/QWidget>
#include "ui_MyViewTest.h"

class MyViewTest : public QWidget
{
    Q_OBJECT

public:
    MyViewTest(QWidget *parent = nullptr);
    ~MyViewTest();
	QString DescribeGraphicsItem(QGraphicsItem *tag);
private:
	void UpdateMSG();
    Ui::MyViewTestClass ui;
	bool SearchTagIndex = false;
};
