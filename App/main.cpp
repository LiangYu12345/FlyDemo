#include <QApplication>
#include "mapwidget.h"
#include "mapwidgetWingloong2.h"
#include <QDebug>

int main(int argc,char * argv[])
{
    QApplication a(argc,argv);

//    MapWidget w;
//    w.show();

    MapWidgetWingloong2 h;
    h.show();

    return  a.exec();
}
