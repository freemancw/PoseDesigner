/*
===============================================================================
File:       Main.cpp
Author:     Clinton Freeman
Created:    May 13, 2011
===============================================================================
*/

// Qt
#include <QtGui/QApplication>
#include <QSplashScreen>
#include <QCleanlooksStyle>

// local
#include <MainWindow.h>
#include <KinectInfo.h>

static const char* splashText  = "Initializing Kinect...";

int main(int argc, char *argv[])
{
    QApplication pdApp(argc, argv);

    QPixmap pixmap(":/images/gui/splash/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage(splashText, Qt::AlignBottom | Qt::AlignLeft, Qt::white);

    pdApp.processEvents();

    initKinect();

    MainWindow pdWindow;
    pdWindow.showMaximized();

    splash.finish(&pdWindow);
    return pdApp.exec();
}
