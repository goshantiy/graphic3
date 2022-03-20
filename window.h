#ifndef WINDOW_H
#define WINDOW_H

#include <QtGui>
#include <QWidget>
#include <QPushButton>
#include "area.h"

class Window : public QWidget
{
protected:
 QTextCodec *codec;
 Area * area; // область отображения рисунка
 QPushButton * btn;
public:
 Window();
};
#endif

