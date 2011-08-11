/*
===============================================================================
File:       Main.cpp
Author:     Clinton Freeman
Created:    May 13, 2011

Comments:   This file doesn't really do much in terms of useful functionality.
            The main point of interest is the splash screen - it would be neat
            to have a progress bar that is linked to actual loading - not sure
            how to do that yet.
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
    //pugi::xml_document doc;
    //pugi::xml_parse_result result = doc.load_file("C:/test.xml");

    ///qDebug("Load result: %s, pose name: %s\n", result.description(), doc.child("pose").attribute("name").value());

    //<< "Load result: " << result.description() << ", pose name: " << doc.child("pose").attribute("name").value() << std::endl;

    QApplication pdApp(argc, argv);

    QPixmap splashGraphic(":/images/gui/splash/splash.png");
    QSplashScreen splash(splashGraphic);
    splash.show();
    splash.showMessage(splashText, Qt::AlignBottom | Qt::AlignLeft, Qt::white);

    MainWindow pdWindow;
    pdWindow.showMaximized();

    splash.finish(&pdWindow);

    return pdApp.exec();
}
