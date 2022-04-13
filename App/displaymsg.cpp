#include "displaymsg.h"
#include <QDebug>
#include <QHBoxLayout>

MyDisplayMsg::MyDisplayMsg(QWidget *parent)
    :QWidget(parent)
{
    Init();
}

MyDisplayMsg::~MyDisplayMsg()
{

}

void MyDisplayMsg::setAltValue(double val)
{
    m_labelaltvalue->setText(QString::number(val));
}

void MyDisplayMsg::setLonValue(double val)
{
    m_labellonvalue->setText(QString::number(val));
}

void MyDisplayMsg::setLatValue(double val)
{
    m_labellatvalue->setText(QString::number(val));
}

void MyDisplayMsg::setHHRValue(osg::Vec3d val)
{
    m_labelhhrxvalue->setText(QString::number(val.x()));
    m_labelhhryvalue->setText(QString::number(val.y()));
    m_labelhhrzvalue->setText(QString::number(val.z()));
}


void MyDisplayMsg::Init()
{
    QHBoxLayout * layout = new QHBoxLayout();
    m_labelalt = new QLabel(u8"高度");
    m_labellon = new QLabel(u8"经度");
    m_labellat = new QLabel(u8"纬度");
    m_labelhhrx = new QLabel(u8"姿态X");
    m_labelhhry = new QLabel(u8"姿态Y");
    m_labelhhrz = new QLabel(u8"姿态Z");
    m_labelaltvalue = new QLabel;
    m_labelaltvalue->setFixedSize(100,20);
    m_labellonvalue = new QLabel;
    m_labellonvalue->setFixedSize(100,20);
    m_labellatvalue = new QLabel;
    m_labellatvalue->setFixedSize(100,20);
    m_labelhhrxvalue = new QLabel;
    m_labelhhrxvalue->setFixedSize(100,20);
    m_labelhhryvalue = new QLabel;
    m_labelhhryvalue->setFixedSize(100,20);
    m_labelhhrzvalue = new QLabel;
    m_labelhhrzvalue->setFixedSize(100,20);

    layout->addWidget(m_labelalt);
    layout->addWidget(m_labelaltvalue);

    layout->addWidget(m_labellon);
    layout->addWidget(m_labellonvalue);

    layout->addWidget(m_labellat);
    layout->addWidget(m_labellatvalue);

    layout->addWidget(m_labelhhrx);
    layout->addWidget(m_labelhhrxvalue);

    layout->addWidget(m_labelhhry);
    layout->addWidget(m_labelhhryvalue);

    layout->addWidget(m_labelhhrz);
    layout->addWidget(m_labelhhrzvalue);

    this->setLayout(layout);

}
