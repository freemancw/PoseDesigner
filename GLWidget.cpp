/*!
 *  @file GLWidget.cpp
 *  @author Clinton Freeman
 *  @date 5/13/2011
 *
 *  Widget that provides an OpenGL context to draw Kinect data. Hands off
 *  most of the actual work to a KinectDrawer object.
 *
 *  @brief Widget for displaying Kinect data (implementation)
 */

// Qt
#include <QtGui>
#include <QtOpenGL>
#include <QTimer>

// local
#include <GLWidget.h>
#include <KinectDrawer.h>

static KinectDrawer kd;

static const int REDRAW_MSEC = 60;
static const int MINHINT_WIDTH = 64;
static const int MINHINT_HEIGHT = 64;
static const int PREFHINT_WIDTH = 512;
static const int PREFHINT_HEIGHT = 512;

/*!
 *  @brief Creates a timer to constantly redraw this widget.
 */
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    kd = KinectDrawer();

    // create timer to constantly redraw this widget
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(REDRAW_MSEC);
}

/*!
 *  @brief Performs no cleanup at the moment.
 *  @todo Should this do anything?
 */
GLWidget::~GLWidget() {}

/*!
 *  @brief Provides the GUI system with a preferable minimum size.
 */
QSize GLWidget::minimumSizeHint() const
{
    return QSize(MINHINT_WIDTH, MINHINT_HEIGHT);
}

/*!
 *  @brief Provides the GUI system with a preferable size.
 */
QSize GLWidget::sizeHint() const
{
    return QSize(PREFHINT_WIDTH, PREFHINT_HEIGHT);
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
