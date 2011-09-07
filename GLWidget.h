/*!
 *  @file GLWidget.h
 *  @author Clinton Freeman
 *  @date 5/13/2011
 *
 *  Widget for displaying Kinect data with OpenGL. Nothing particularly
 *  interesting here.
 */

#ifndef GLWIDGET_H
#define GLWIDGET_H

// Qt
#include <QGLWidget>

/*!
 *  @class GLWidget
 *  Description goes here.
 */
class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // GLWIDGET_H
