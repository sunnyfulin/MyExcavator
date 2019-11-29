#include "Widget.h"

#include <QtGui>
#include <QLayout>
#include <QDebug>
#include <QPainterPath>
#include <QLabel>

#define Q_(s) QStringLiteral(s)

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , _scene(nullptr)
    , _view(nullptr)
{
    _scene = new MyScene;

    _view = new QGraphicsView(_scene);
    _view->setRenderHint(QPainter::Antialiasing);

    _bodyMoveSlider = new QSlider(Qt::Horizontal);
    _bodyMoveSlider->setRange(-100,40);
    connect(_bodyMoveSlider, &QSlider::valueChanged, _scene, &MyScene::OnBodyMove);

    _bigHandAngleSlider = new QSlider(Qt::Horizontal);
    _bigHandAngleSlider->setRange(0, 40);
    connect(_bigHandAngleSlider, &QSlider::valueChanged, _scene, &MyScene::OnBigHandRotate);

    _smallHandAngleSlider = new QSlider(Qt::Horizontal);
    _smallHandAngleSlider->setRange(0,100);
    connect(_smallHandAngleSlider, &QSlider::valueChanged, _scene, &MyScene::OnSmallHandRotate);

    _littleHandAngleSlider = new QSlider(Qt::Horizontal);
    _littleHandAngleSlider->setRange(0,80);
    connect(_littleHandAngleSlider, &QSlider::valueChanged, _scene, &MyScene::OnLittleHandRotate);

    QVBoxLayout * lyt = new QVBoxLayout(this);
    lyt->addWidget(_view);

    QHBoxLayout * bodyLyt = new QHBoxLayout;
    bodyLyt->addWidget(new QLabel(Q_("车体")));
    bodyLyt->addWidget(_bodyMoveSlider);

    QHBoxLayout * bigLyt = new QHBoxLayout;
    bigLyt->addWidget(new QLabel(Q_("大臂")));
    bigLyt->addWidget(_bigHandAngleSlider);

    QHBoxLayout * smallLyt = new QHBoxLayout;
    smallLyt->addWidget(new QLabel(Q_("小臂")));
    smallLyt->addWidget(_smallHandAngleSlider);

    QHBoxLayout * littleLyt = new QHBoxLayout;
    littleLyt->addWidget(new QLabel(Q_("爪子")));
    littleLyt->addWidget(_littleHandAngleSlider);

    lyt->addLayout(bodyLyt);
    lyt->addLayout(bigLyt);
    lyt->addLayout(smallLyt);
    lyt->addLayout(littleLyt);

    resize(1000,800);
    setWindowTitle(Q_("挖掘机操作实习"));
}

Widget::~Widget()
{
    if(_scene)
    {
        delete _scene;
        _scene = nullptr;
    }

    if(_view)
    {
        delete _view;
        _view = nullptr;
    }
}

//===========================================================================
QRectF Hand::boundingRect() const
{
    return QRectF(-200,-200,600,600);
}

void Hand::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(Qt::yellow);

    QPainterPath path;
    path.moveTo(-12, -30);
    path.lineTo(-3, -35);
    path.lineTo(-6, -44);
    path.lineTo(6, -48);
    path.lineTo(35, -45);
    path.lineTo(70, 0);
    path.lineTo(15, -10);
    path.lineTo(10, 0);
    path.lineTo(3, 7);
    path.lineTo(-3, 3);
    path.lineTo(-12, -30);
    painter->drawPath(path);

    painter->setBrush(Qt::darkYellow);
    painter->drawEllipse(PinPoint,5,5);
    painter->drawEllipse(_pinPoint1,5,5);
}

//===========================================================================
QRectF LittleHand::boundingRect() const
{
    return QRectF(-200,-200,600,600);
}

void LittleHand::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(QBrush(Qt::black), 10));
    painter->drawLine(0,0, PinPoint.x(), PinPoint.y());

    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(QBrush(Qt::darkYellow));
    painter->drawEllipse(-5, -5, 10, 10);
    painter->drawEllipse(PinPoint, 5,5);
}

//===========================================================================
QRectF SmallHand::boundingRect() const
{
    return QRectF(-200,-200,600,600);
}

void SmallHand::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(Qt::yellow);

    QPainterPath path;
    path.moveTo(-50, -30);
    path.lineTo(200, 0);
    path.lineTo(198, 10);
    path.lineTo(-3, 5);
    path.lineTo(-55, -18);
    path.lineTo(-50, -30);

    path.moveTo(_pinPoint1.x()-10, _pinPoint1.y()+5);
    path.lineTo(_pinPoint1.x(), _pinPoint1.y()-6);
    path.lineTo(_pinPoint1.x()+10, _pinPoint1.y()+7);
    path.lineTo(_pinPoint1.x()-8, _pinPoint1.y()+5);
    painter->drawPath(path);

    painter->setBrush(Qt::darkYellow);
    painter->drawEllipse(-5,-5, 10, 10);
    painter->drawEllipse(PinPoint,5,5);
    painter->drawEllipse(_pinPoint1,5,5);
    painter->drawEllipse(_pinPoint2,5,5);
    painter->drawEllipse(_pinPoint3,5,5);

    painter->setPen(QPen(QBrush(Qt::gray), 6));
    QPointF p(mapFromItem(_littleHand, _littleHand->PinPoint));
    painter->drawLine(_pinPoint1.x(), _pinPoint1.y(), p.x(), p.y());

    //液压臂真麻烦，高中直线方程没学好
    painter->setPen(QPen(QBrush(Qt::black), 10));
    QLineF line(_pinPoint1, p);
    qreal angle = line.angle();
    QPointF midPoint(_pinPoint1.x()+ 90*cos(qDegreesToRadians(angle)), _pinPoint1.y() - 90*sin(qDegreesToRadians(angle)));
    painter->drawLine(_pinPoint1, midPoint);

    painter->setPen(QPen(QBrush(Qt::black), 10));
    QPointF p1(mapFromItem(_hand, _hand->PinPoint));
    painter->drawLine(p.x(), p.y(), p1.x(), p1.y());
}

//===========================================================================
QRectF BigHand::boundingRect() const
{
    return QRectF(-200,-200,600,600);
}

void BigHand::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(Qt::yellow);

    QPainterPath path;
    path.moveTo(-6,2);
    path.lineTo(60,-180);
    path.lineTo(250,-270);
    path.lineTo(258,-260);
    path.lineTo(90,-150);
    path.lineTo(5,5);

    path.moveTo(_pinPoint2.x()-10, _pinPoint2.y()+5);
    path.lineTo(_pinPoint2.x()-5, _pinPoint2.y()-8);
    path.lineTo(_pinPoint2.x()+8, _pinPoint2.y()-5);
    path.lineTo(_pinPoint2.x()-10, _pinPoint2.y()+5);
    painter->drawPath(path);

    painter->setBrush(Qt::darkYellow);
    painter->drawEllipse(-4,-5,10,10);
    painter->drawEllipse(PinPoint,5,5);

    painter->drawEllipse(_pinPoint1,5,5);

    painter->drawEllipse(_pinPoint2,5,5);

    painter->setPen(QPen(QBrush(Qt::gray), 6));
    QPointF p(mapFromItem(_smallHand, _smallHand->PinPoint));
    painter->drawLine(_pinPoint2.x(), _pinPoint2.y(), p.x(), p.y());

    //液压臂
    painter->setPen(QPen(QBrush(Qt::black), 10));
    QLineF line(_pinPoint2, p);
    qreal angle = line.angle();
    QPointF midPoint(_pinPoint2.x()+ 110*cos(qDegreesToRadians(angle)), _pinPoint2.y() - 110*sin(qDegreesToRadians(angle)));
    painter->drawLine(_pinPoint2, midPoint);

}

//===========================================================================
QRectF Body::boundingRect() const
{
    return QRectF(-200,-200,600,600);
}

void Body::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(QBrush(Qt::darkYellow));
    painter->drawRect(0,0,220,100);
    painter->drawRect(100,-80,100,80);

    painter->setBrush(QBrush(Qt::darkYellow));
    painter->drawEllipse(_pinPoint1, 5,5);
    painter->drawEllipse(_pinPoint2, 5,5);

    painter->setPen(QPen(QBrush(Qt::gray), 6));
    QPointF p(mapFromItem(_bigHand, _bigHand->PinPoint));
    painter->drawLine(_pinPoint2.x(), _pinPoint2.y(), p.x(), p.y());

    //液压臂
    painter->setPen(QPen(QBrush(Qt::black), 10));
    QLineF line(_pinPoint2, p);
    qreal angle = line.angle();
    QPointF midPoint(_pinPoint2.x()+ 80*cos(qDegreesToRadians(angle)), _pinPoint2.y() - 80*sin(qDegreesToRadians(angle)));
    painter->drawLine(_pinPoint2, midPoint);
}
