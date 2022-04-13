#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include "interactivemap.h"
#include "flightnode.h"
#include "mapobjectitem.h"
#include "maprouteitem.h"
#include <QMouseEvent>
#include "interactivemap.h"
#include "mapoperator.h"

class MapWidget : public QWidget
{
    Q_OBJECT
public:
   MapWidget(QWidget * parent = nullptr);
   ~MapWidget();

   void CreateFlight(int id);
   void LinkFlightAndRoute(int id,MapRouteItem * item);
private:
   void Init();
   void slotCreateFlight(MapObjectItem * item);
   void slotCreateRoute(MapRouteItem * item);
   void slotCreateFinish();
private:
    InteractiveMap * m_viewMap;

    FlightNode * m_flynode;
    MapObjectItem * m_flymapnode;
    MapRouteItem * m_flyroute;

    QPushButton * m_stopBtn;
    QPushButton * m_continueBtn;
    QPushButton * m_changeSpeedBtn;
    QLineEdit * m_lineSpeed;
    QPushButton * m_routeCreateBtn;

    QElapsedTimer m_timer;

    MapObjectOperator m_objectOperator;

    int id = 1;   //编号
    QVector<FlightNode *> m_vflynodes;
    QMap<int,MapObjectItem *> m_vflymapnodes;
    QMap<int,MapRouteItem *> m_vflyroutes;
};
