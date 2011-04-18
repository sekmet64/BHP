#pragma once

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "SideWidget.h"
#include "GLWidget.h"
#include <QHBoxLayout>
#include <QScrollArea>

class MainWindow: public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

private:
    SideWidget  *_side_widget;      // pointer to our side widget
    GLWidget    *_gl_widget;        // pointer to our rendering OpenGL widget

    QScrollArea *_scroll_area;      // provides a scrolling view onto our side widget
    QHBoxLayout *_layout;           // a horizontal layout that consists of _gl_widget and _scroll_area
    QWidget     *_central_widget;   // pointer to a widget whose layout manager will be set to _layout

public:
    // special and default constructor
    MainWindow(QWidget *parent = 0);

private slots:
    // private event handling methods/slots
    void on_action_Quit_triggered();
};
