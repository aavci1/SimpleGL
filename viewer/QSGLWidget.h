#ifndef QSGLWIDGET_H
#define QSGLWIDGET_H

#include <QGLWidget>

namespace SimpleGL {
  class Window;
}

class QSGLWidgetPrivate;

class QSGLWidget : public QGLWidget {
  Q_OBJECT
public:
  QSGLWidget(QWidget *parent = 0);
  ~QSGLWidget();

  SimpleGL::Window *sglWindow() const;

protected:
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();

  void keyPressEvent(QKeyEvent *e);
  void keyReleaseEvent(QKeyEvent *e);

  void mouseMoveEvent(QMouseEvent *e);
  void mousePressEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);

  void wheelEvent(QWheelEvent *e);

signals:
  void initialized();

  void keyPressed(QKeyEvent *e);
  void keyReleased(QKeyEvent *e);

  void mouseMoved(QMouseEvent *e);
  void mousePressed(QMouseEvent *e);
  void mouseReleased(QMouseEvent *e);

  void wheelMoved(QWheelEvent *e);

private:
  QSGLWidgetPrivate *d;
};

#endif // QSGLWIDGET_H
