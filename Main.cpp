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
#include <QPlastiqueStyle>

// local
#include <MainWindow.h>
#include <KinectInfo.h>
#include <PoseSample.h>

static const char* splashText  = "Initializing Kinect...";

int main(int argc, char* argv[])
{
    QApplication pdApp(argc, argv);

    // necessary for the tablewidget to display correctly
    QString style =
    "QHeaderView::section, QTableView QTableCornerButton::section { "
    "color: #fff; padding: 4px; border: 1px solid #444; "
    "background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, "
    "stop:0 rgba(80, 80, 80, 255), stop:1 rgba(90, 90, 90, 255)); } ";
    //pdApp.setStyleSheet(style);

    QPixmap splashGraphic(":/images/gui/splash/splash.png");
    QSplashScreen splash(splashGraphic);
    splash.show();
    splash.showMessage(splashText, Qt::AlignBottom | Qt::AlignLeft, Qt::white);

    MainWindow pdWindow;
    pdWindow.showMaximized();

    splash.finish(&pdWindow);

    // need to embed font, most systems will not have Pacifico
    QFontDatabase::addApplicationFont(":/fonts/gui/fonts/Pacifico.ttf");

    PoseSample::unitTest();

    return pdApp.exec();
}
