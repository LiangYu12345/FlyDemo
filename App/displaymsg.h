#pragma once

#include <QWidget>
#include <QLabel>
#include <osg/Vec3d>

class MyDisplayMsg : public QWidget
{
    Q_OBJECT
public:
   MyDisplayMsg(QWidget * parent = nullptr);
   ~MyDisplayMsg();

   void setAltValue(double val);
   void setLonValue(double val);
   void setLatValue(double val);
   void setHHRValue(osg::Vec3d val);
private:
   void Init();
private:
   QLabel * m_labelalt;
   QLabel * m_labellon;
   QLabel * m_labellat;
   QLabel * m_labelhhrx;
   QLabel * m_labelhhry;
   QLabel * m_labelhhrz;
   QLabel * m_labelaltvalue;
   QLabel * m_labellonvalue;
   QLabel * m_labellatvalue;
   QLabel * m_labelhhrxvalue;
   QLabel * m_labelhhryvalue;
   QLabel * m_labelhhrzvalue;
};
