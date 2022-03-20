#include "area.h"
#define PI 3.14159265

Area::Area(QWidget *parent):QWidget(parent)
{
setFixedSize(QSize(1000,800));
myFigura = new Figura(9);

}
void Area::showEvent(QShowEvent *)
{
 myTimer=startTimer(50);
}
void Area::paintEvent(QPaintEvent *event)
{    Q_UNUSED(event);
     QPainter painter(this);
    myFigura->draw(&painter);


}
void Area::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == myTimer)
    {
        double x = obsCoord[0];
        double angle = 2 * PI /180;
        obsCoord[0] = x * cos(angle) - obsCoord[1] * sin(angle);
        obsCoord[1] = x * sin(angle) + obsCoord[1] * cos(angle);
//        obsCoord[0] += 1;
//        obsCoord[1] -= 1;
        myFigura->computeCoordinates(obsCoord);
        myFigura->computeBodyMatrix();
        myFigura->checkGran(obsCoord);
        update();
    } else {
        QWidget::timerEvent(event);
    }
}
void Area::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    killTimer(myTimer);
}
Area::~Area()
{
    delete myFigura;
}
