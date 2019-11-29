#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QSlider>

//===========================================================================
class Hand : public QGraphicsItem
{
public:
    Hand(QGraphicsItem * parent = nullptr)
        : QGraphicsItem(parent)
        , PinPoint(-6, -27)
        , _pinPoint1(1, 1)
    {
    }

    void setRotateAngle(qreal angle)
    {
        prepareGeometryChange();
        setRotation(angle);
    }

    const QPointF PinPoint;

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    const QPointF _pinPoint1;
};

//===========================================================================
class LittleHand : public QGraphicsItem
{
public:
    LittleHand(QGraphicsItem * parent = nullptr)
        :QGraphicsItem(parent)
        , PinPoint(-9,-28)
    {

    }

    void setRotateAngle(qreal angle)
    {
        prepareGeometryChange();
        setRotation(angle);
    }

    const QPointF PinPoint;

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    const QPointF _pinPoint1;
};

//===========================================================================
class SmallHand : public QGraphicsItem
{
public:
    SmallHand(QGraphicsItem * parent = nullptr)
        :QGraphicsItem(parent)
        , PinPoint(-50, -23)
        , _pinPoint1(50, -23)
        , _pinPoint2(200, 5)
        , _pinPoint3(180, 3)
    {
        _littleHand = new LittleHand(this);
        _littleHand->setParentItem(this);
        _littleHand->setPos(_pinPoint3);
        _littleHand->setRotateAngle(0);

        _hand = new Hand(this);
        _hand->setParentItem(this);
        _hand->setPos(_pinPoint2);
        _hand->setRotateAngle(0);
    }

    void setRotateAngle(qreal angle)
    {
        prepareGeometryChange();
        setRotation(angle);
    }

    const QPointF PinPoint;

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    LittleHand * _littleHand;
    Hand * _hand;

    const QPointF _pinPoint1;
    const QPointF _pinPoint2;
    const QPointF _pinPoint3;
};

//===========================================================================
class BigHand : public QGraphicsItem
{
public:
    BigHand(QGraphicsItem * parent = nullptr)
        : QGraphicsItem(parent)
        , PinPoint(100, -165)
        , _pinPoint1(254, -265)
        , _pinPoint2(100, -200)
    {
        _smallHand = new SmallHand(this);
        _smallHand->setPos(_pinPoint1);
        _smallHand->setParentItem(this);

        _smallHand->setRotateAngle(0);
    }

    void setRotateAngle(qreal angle)
    {
        prepareGeometryChange();
        setRotation(angle);
    }

    const QPointF PinPoint;

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    SmallHand * _smallHand;

    const QPointF _pinPoint1;
    const QPointF _pinPoint2;
};


//===========================================================================
class Body : public QGraphicsItem
{
public:
    Body(QGraphicsItem * parent = nullptr)
        : QGraphicsItem(parent)
        , _pinPoint1(160,0)
        , _pinPoint2(200,0)
    {
        _bigHand = new BigHand(this);
        _bigHand->setParentItem(this);
        _bigHand->setPos(_pinPoint1);

        _bigHand->setRotateAngle(0);

    }

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    const QPointF _pinPoint1;
    const QPointF _pinPoint2;
    BigHand * _bigHand;
};

class MyScene : public QGraphicsScene
{
    Q_OBJECT

public:
    MyScene(){
        _body = new Body;
        _body->setPos(0, 350);
        addItem(_body);

        setSceneRect(0,0,600,400);
    }

public slots:
    void OnBodyMove(qreal moveDistance)
    {
        _body->setPos(moveDistance,350);
    }

    void OnBigHandRotate(qreal angle)
    {
        BigHand * bh = qgraphicsitem_cast<BigHand*>(_body->childItems().at(0));
        bh->setRotateAngle(angle);

        advance();
    }

    void OnSmallHandRotate(qreal angle)
    {
       SmallHand * sh = qgraphicsitem_cast<SmallHand*>(_body->childItems().at(0)->childItems().at(0));
       sh->setRotateAngle(angle);

       advance();
    }

    void OnLittleHandRotate(qreal angle)
    {
       LittleHand * lh = qgraphicsitem_cast<LittleHand*>(_body->childItems().at(0)->childItems().at(0)->childItems().at(0));
       lh->setRotateAngle(angle);

       Hand * h = qgraphicsitem_cast<Hand*>(_body->childItems().at(0)->childItems().at(0)->childItems().at(1));
       h->setRotateAngle(angle+10);

       advance();
    }

private:
    Body * _body;
};


//===========================================================================
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    MyScene * _scene;
    QGraphicsView * _view;

    QSlider * _bodyMoveSlider;
    QSlider * _bigHandAngleSlider;
    QSlider * _smallHandAngleSlider;
    QSlider * _littleHandAngleSlider;
};

#endif // WIDGET_H
