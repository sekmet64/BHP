#include <QApplication>
#include "MainWindow.h"

int main(int argc, char** argv)
{
    // creating an application object and setting its style
    QApplication app(argc, argv);
    app.setStyle("cleanlooks");

    // creating a main window object
    MainWindow mwnd;
    mwnd.showMaximized();

    // running the application
    return app.exec();
}
