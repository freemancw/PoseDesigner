/*
===============================================================================
File:       GLWidget.cpp
Author:     Clinton Freeman
Created:    May 13, 2011
===============================================================================
*/

// Qt includes
#include <QtGui>
#include <QtOpenGL>
#include <QTimer>

// local includes
#include <GLWidget.h>
#include <KinectDrawer.h>

static KinectDrawer kd;

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    kd = KinectDrawer();

    // create timer to constantly redraw this widget
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(60);
}

GLWidget::~GLWidget() {}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(64, 64);
}

QSize GLWidget::sizeHint() const
{
    return QSize(512, 512);
}

void GLWidget::initializeGL()
{
    kd.initializeGL();
}

void GLWidget::paintGL()
{
    kd.drawFrame();
}

void GLWidget::resizeGL(int width, int height)
{
    int side;

    side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::mousePressEvent(QMouseEvent *event){}
void GLWidget::mouseMoveEvent(QMouseEvent *event){}
