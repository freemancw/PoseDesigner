#-------------------------------------------------
#
# Project created by QtCreator 2011-05-11T17:27:04
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
QT       += xml

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
    Pose.cpp \
    HelpDialog.cpp \
    SkeletonVector.cpp \
    SCoord.cpp \
    ALGLIB/statistics.cpp \
    ALGLIB/specialfunctions.cpp \
    ALGLIB/solvers.cpp \
    ALGLIB/optimization.cpp \
    ALGLIB/linalg.cpp \
    ALGLIB/interpolation.cpp \
    ALGLIB/integration.cpp \
    ALGLIB/fasttransforms.cpp \
    ALGLIB/diffequations.cpp \
    ALGLIB/dataanalysis.cpp \
    ALGLIB/ap.cpp \
    ALGLIB/alglibmisc.cpp \
    ALGLIB/alglibinternal.cpp

HEADERS  += \
    GLWidget.h \
    MainWindow.h \
    KinectInfo.h \
    KinectDrawer.h \
    KinectOptionsDialog.h \
    PoseSample.h \
    AboutDialog.h \
    Pose.h \
    SkeletonVector.h \
    HelpDialog.h \
    SCoord.h \
    ALGLIB/dataanalysis.h \
    ALGLIB/stdafx.h \
    ALGLIB/statistics.h \
    ALGLIB/specialfunctions.h \
    ALGLIB/solvers.h \
    ALGLIB/optimization.h \
    ALGLIB/linalg.h \
    ALGLIB/interpolation.h \
    ALGLIB/integration.h \
    ALGLIB/fasttransforms.h \
    ALGLIB/diffequations.h \
    ALGLIB/ap.h \
    ALGLIB/alglibmisc.h \
    ALGLIB/alglibinternal.h

FORMS    += mainwindow.ui \
    kinectoptionsdialog.ui \
    aboutdialog.ui \
    helpdialog.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    iconresource.rc \
    tasklist.tasks \
    posedesigner.ico \
    README.markdown \
    installer/installer_script.nsi \
    installer/gpl.txt \
    gui/fonts/Pacifico.ttf \
    gui/icons/uparrow.png \
    gui/icons/undo.png \
    gui/icons/timer.png \
    gui/icons/selectall.png \
    gui/icons/saveas.png \
    gui/icons/save.png \
    gui/icons/rightarrow.png \
    gui/icons/remove.png \
    gui/icons/redo.png \
    gui/icons/question.png \
    gui/icons/paste.png \
    gui/icons/open.png \
    gui/icons/nosample.png \
    gui/icons/new.png \
    gui/icons/math.png \
    gui/icons/leftarrow.png \
    gui/icons/kinect.png \
    gui/icons/information.png \
    gui/icons/export.png \
    gui/icons/exit.png \
    gui/icons/downarrow.png \
    gui/icons/delete.png \
    gui/icons/cut.png \
    gui/icons/copy.png \
    gui/icons/add.png \
    gui/logos/posedesigner.png \
    gui/logos/boogie.png \
    gui/psd/splash.psd \
    gui/psd/icon.psd \
    gui/psd/icon.png \
    gui/splash/splash.png \
    gui/textures/texturedbackground.png

RC_FILE = iconresource.rc






















