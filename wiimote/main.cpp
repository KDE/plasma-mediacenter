

#include <QtGui>
#include <kdebug.h>

//#include <KApplication>
#include <KMainWindow>

#include "wiimotewidget.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    KMainWindow* mainWindow = new KMainWindow();

    WiimoteWidget* wiimoteWidget = new WiimoteWidget(mainWindow);
    mainWindow->setCentralWidget(wiimoteWidget);
    mainWindow->show();

    return app.exec();
}
