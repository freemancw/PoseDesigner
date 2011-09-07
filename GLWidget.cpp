/*!
 *  @file GLWidget.cpp
 *  @author Clinton Freeman
 *  @date 5/13/2011
 *
 *  Widget that provides an OpenGL context to draw Kinect data. Hands off
 *  most of the actual work to a KinectDrawer object.
 */

// Qt
#include <QtGui>
#include <QtOpenGL>
#include <QTimer>

// local
#include <GLWidget.h>
#include <KinectDrawer.h>

static KinectDrawer kd;

/*!
 *  @brief Creates a timer to constantly redraw this widget.
 *  @todo Get rid of magic number (60).
 */
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    kd = KinectDrawer();

    // create timer to constantly redraw this widget
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(60);
}

/*!
 *  @brief Performs no cleanup at the moment.
 *  @todo Should this do anything?
 */
GLWidget::~GLWidget() {}

/*!
 *  @brief Provides the GUI system with a preferable minimum size.
 *  @todo Get rid of magic numbers (64, 64).
 */
QSize GLWidget::minimumSizeHint() const
{
    return QSize(64, 64);
}

/*!
 *  @brief Provides the GUI system with a preferable size in general.
 *  @todo Get rid of magic numbers (512, 512).
 */
QSize GLWidget::sizeHint() const
{
    return QSize(512, 512);
}

/*!
 *  @brief Forwards GL initialization to the KinectDrawer object.
 */
void GLWidget::initializeGL()
{
    kd.initializeGL();
}

//! @brief Has the KinectDrawer object draw a single frame.
void GLWidget::paintGL()
{
    kd.drawFrame();
}

//! @brief Handles window resizing.
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
