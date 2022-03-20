#ifndef AREA_H
#define AREA_H
#include "figura.h"
#include <QWidget>

class Area : public QWidget
{
    int myTimer; // идентификатор таймера
    double obsCoord[3] = {10,10,10};
public:
     Area(QWidget *parent = 0);
    ~Area();
     Figura * myFigura;
protected:
     void paintEvent(QPaintEvent *event);
     void timerEvent(QTimerEvent *event);
     void showEvent(QShowEvent *event);
     void hideEvent(QHideEvent *event);
};
#endif // AREA_H
