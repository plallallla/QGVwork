#include "MyViewTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyViewTest w;
    w.show();
    return a.exec();
}
