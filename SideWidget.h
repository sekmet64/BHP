#pragma once

#include <QWidget>
#include "ui_SideWidget.h"

class SideWidget: public QWidget, public Ui::SideWidget
{
public:
    // special and default constructor
    SideWidget(QWidget *parent = 0);
};
