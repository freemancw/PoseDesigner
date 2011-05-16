/*
===============================================================================
File:       Main.cpp
Author:     Clinton Freeman
Created:    May 13, 2011
===============================================================================
*/

// Qt includes
#include <QtGui/QApplication>
#include <QSplashScreen>
#include <QCleanlooksStyle>

// local includes
#include <MainWindow.h>
#include <KinectInfo.h>

static const char* splashImage = "C:/Users/Clinton/Desktop/Splash.bmp";
static const char* splashText  = "Initializing Kinect...";

int main(int argc, char *argv[])
{
    QApplication pdApp(argc, argv);
    //QApplication::setStyle(new QCleanlooksStyle);

    // create splash screen and show it
    QPixmap pixmap(splashImage);
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage(splashText, Qt::AlignBottom | Qt::AlignLeft);
    pdApp.processEvents();

    // initialize connection with Kinect
    initKinect();

    // create the main window and show it
    MainWindow pdWindow;
    pdWindow.showMaximized();

    // remove splash screen and display main window
    splash.finish(&pdWindow);
    return pdApp.exec();
}
