#include "mapwidget.h"
#include <QVBoxLayout>
#include <QScrollBar>
#include <QSlider>
#include <QOpenGLWidget>
#include "clocktimer.h"
#include <QVector3D>
#include <osg/Vec3d>
#include "simthread.h"
//#include "flightthread.h"
#include "cloudshadownode.h"
#include <QDebug>

MapWidget::MapWidget(QWidget *parent)
    :QWidget(parent)
{
    Init();
}

MapWidget::~MapWidget()
{

}

void MapWidget::CreateFlight(int id)
{
    FlightNode * flynode = SimThread::instance()->getOrCreate(id);

    flynode->createFly(FLIGHT_CLOUDSHAODW);
    flynode->getFly()->setInitTrueAirspeed(250);
    flynode->getFly()->setPhase(PHASE_CRUISE);
    flynode->getFly()->navPoint(1);
    flynode->getFly()->engineStart();
    flynode->getFly()->engineFire();
    flynode->getFly()->autoPilot();
    flynode->getFly()->takeoff();

    connect(flynode->getFly(),&FlyNode::updated,this,[=](){
        auto mapnode = m_vflymapnodes.find(flynode->UID());
        auto node = m_vflynodes.at(flynode->UID() - 1);
        mapnode.value()->setCoordinate(node->getFly()->latLonAlt());
        double angle = node->getFly()->eulerAngle().x();
        mapnode.value()->setRotation(angle);
    });

    m_vflynodes.append(flynode);
}

void MapWidget::LinkFlightAndRoute(int id, MapRouteItem * item)
{
    QVector<Waypoint> waypoints;
    Waypoint temp;
    auto points = item->points();
    foreach(auto Index,points)
    {
        temp.coord = Index.coord;
        qDebug()<<Index.coord.latitude()<<Index.coord.longitude()<<"this";
        temp.speed = Index.speed;
        waypoints.append(temp);
    }
    m_vflynodes.at(id-1)->getFly()->setInitLatLonAlt(waypoints.first().coord);
    osg::Vec3d InitHPR(0,0,0);
    m_vflynodes.at(id-1)->getFly()->setInitEulerAngle(InitHPR);
    m_vflynodes.at(id-1)->getFly()->setCruiseAirway(waypoints);

    m_vflynodes.at(id-1)->getFly()->thread()->start();

    //起飞
    m_vflynodes.at(id-1)->getFly()->start();
}


void MapWidget::Init()
{
    this->resize(1440,810);

    m_viewMap = new InteractiveMap(this);
    m_viewMap->setTilePath("Z:/tms");
    m_viewMap->setZoomable(true);
    m_viewMap->setZoomLevel(5);
    m_viewMap->setTMSMode(true);
    m_viewMap->centerOn(QGeoCoordinate(40,110));
    m_viewMap->setDragMode(QGraphicsView::ScrollHandDrag);
    m_viewMap->setRenderHint(QPainter::HighQualityAntialiasing,true);
    m_viewMap->setOptimizationFlag(QGraphicsView::DontSavePainterState);
    m_viewMap->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    m_viewMap->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    m_viewMap->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    m_viewMap->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    
    QWidget * menuwidget = new QWidget(this);

    m_stopBtn = new QPushButton(menuwidget);
    m_stopBtn->setText(tr(u8"暂停"));
    m_continueBtn = new QPushButton(menuwidget);
    m_continueBtn->setText(tr(u8"继续"));
    m_continueBtn->setEnabled(false);
    m_lineSpeed = new QLineEdit(menuwidget);
    m_lineSpeed->setText(tr("1"));
    m_changeSpeedBtn = new QPushButton(menuwidget);
    m_changeSpeedBtn->setText(tr(u8"变速"));
    m_routeCreateBtn = new QPushButton(menuwidget);
    m_routeCreateBtn->setText(tr(u8"创建航线"));

    connect(m_stopBtn,&QPushButton::clicked,this,[=](){
        ClockTimer::instance()->stop();
        m_continueBtn->setEnabled(true);
        m_stopBtn->setEnabled(false);
    });
    connect(m_continueBtn,&QPushButton::clicked,this,[=](){
        ClockTimer::instance()->start();
        m_continueBtn->setEnabled(false);
        m_stopBtn->setEnabled(true);
    });
    connect(m_changeSpeedBtn,&QPushButton::clicked,this,[=](){
        int speed = m_lineSpeed->text().trimmed().toInt();
        ClockTimer::instance()->speedUp(speed);
    });
    connect(m_routeCreateBtn,&QPushButton::clicked,this,[=](){
        m_viewMap->setOperator(&m_objectOperator);
    });
    connect(&m_objectOperator,static_cast<void(MapObjectOperator::*)(MapObjectItem *)>(&MapObjectOperator::created),this,&MapWidget::slotCreateFlight,Qt::DirectConnection);
    connect(&m_objectOperator,static_cast<void(MapObjectOperator::*)(MapRouteItem*)>(&MapObjectOperator::created),this,&MapWidget::slotCreateRoute,Qt::DirectConnection);
    connect(&m_objectOperator,&MapObjectOperator::finished,this,&MapWidget::slotCreateFinish);

    QHBoxLayout * hLayou = new QHBoxLayout();
    hLayou->addWidget(m_stopBtn);
    hLayou->addWidget(m_continueBtn);
    hLayou->addWidget(m_lineSpeed);
    hLayou->addWidget(m_changeSpeedBtn);
    hLayou->addWidget(m_routeCreateBtn);
    menuwidget->setGeometry(0,0,540,40);
    menuwidget->setLayout(hLayou);

    m_flynode = SimThread::instance()->getOrCreate(1);
    m_flymapnode = m_viewMap->addMapItem<MapObjectItem>();
    m_flymapnode->setIcon(QString(":/new/prefix1/Resources/plane.png"));

    m_vflynodes.append(m_flynode);
    m_vflymapnodes.insert(m_flynode->UID(),m_flymapnode);

    QGeoCoordinate initLLA(40,110,200);
    osg::Vec3d initHPR(0,0,0);
    m_flynode->createFly(FLIGHT_CLOUDSHAODW);
    m_flynode->getFly()->setInitLatLonAlt(initLLA);
    m_flynode->getFly()->setEulerAngle(initHPR);
    m_flynode->getFly()->setInitTrueAirspeed(250);
    m_flynode->getFly()->setPhase(PHASE_CRUISE);
    m_flynode->getFly()->navPoint(1);

    m_flymapnode->setCoordinate(QGeoCoordinate(40,110));
    m_timer.start();
    //关联经纬度
    connect(m_flynode->getFly(),&FlyNode::updated,this,[=](){
        m_flymapnode->setCoordinate(m_flynode->getFly()->latLonAlt());
        double angle = m_flynode->getFly()->eulerAngle().x();
        m_flymapnode->setRotation(angle);
    });

//    connect(ClockTimer::instance(), &ClockTimer::timeout_10, this, [=](){
//        qDebug()<<  m_timer.elapsed();
//        m_timer.restart();
//    });

    m_flynode->getFly()->engineStart();
    m_flynode->getFly()->engineFire();
    m_flynode->getFly()->autoPilot();
    m_flynode->getFly()->takeoff();

    QVector<Waypoint> waypoints;
    Waypoint temp;
    int alt = 202;
    int lat = 40;
    int lon = 110;

    for(int i = 0; i < 10 ;i ++)
    {
        temp.coord.setAltitude(alt);
        temp.coord.setLatitude(lat);
        temp.coord.setLongitude(lon);
        alt ++;
        lat ++;
        lon ++;
        waypoints.append(temp);
    }
    m_flynode->getFly()->setCruiseAirway(waypoints);

    m_flyroute = new MapRouteItem;
    foreach(auto it,waypoints)
    {
        MapRouteItem::Point point;
        point.coord = it.coord;
        point.speed = 250;
        m_flyroute->append(point);
    }
    m_viewMap->scene()->addItem(m_flyroute);
     m_vflyroutes.insert(m_flynode->UID(),m_flyroute);

    m_flynode->getFly()->start();
    SimThread::instance()->start();
    ClockTimer::instance()->start();
}

void MapWidget::slotCreateFlight(MapObjectItem *item)
{
    item->setIcon(QString(":/new/prefix1/Resources/plane.png"));

    id++;
    m_vflymapnodes.insert(id,item);
    CreateFlight(id);
}

void MapWidget::slotCreateRoute(MapRouteItem *item)
{
    item->setEditable(true);
    m_vflyroutes.insert(id,item);
}

void MapWidget::slotCreateFinish()
{
    //关联飞机-航线
    LinkFlightAndRoute(id,m_vflyroutes.find(id).value());
}
