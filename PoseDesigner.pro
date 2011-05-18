#-------------------------------------------------
#
# Project created by QtCreator 2011-05-11T17:27:04
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

TARGET = PoseDesigner
TEMPLATE = app

LIBS += -L"C:/Program Files (x86)/OpenNI/Lib" -lopenNI

INCLUDEPATH += "C:/Program Files (x86)/OpenNI/Include"

SOURCES +=\
    GLWidget.cpp \
    Main.cpp \
    MainWindow.cpp \
    KinectInfo.cpp \
    KinectDrawer.cpp \
    KinectOptionsDialog.cpp \
    PoseSample.cpp \
    AboutDialog.cpp \
    Vector3.cpp

HEADERS  += \
    GLWidget.h \
    MainWindow.h \
    KinectInfo.h \
    KinectDrawer.h \
    KinectOptionsDialog.h \
    PoseSample.h \
    AboutDialog.h \
    Vector3.h

FORMS    += mainwindow.ui \
    kinectoptionsdialog.ui \
    aboutdialog.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    iconresource.rc \
    tasklist.tasks

RC_FILE = iconresource.rc
