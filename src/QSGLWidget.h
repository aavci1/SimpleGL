#ifndef QSGLWIDGET_H
#define QSGLWIDGET_H

#include <QGLWidget>

class QSGLWidgetPrivate;

class QSGLWidget : public QGLWidget {
  Q_OBJECT
public:
  QSGLWidget(QWidget *parent = 0);
  ~QSGLWidget();

protected:
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();

  void keyPressEvent(QKeyEvent *e);
  void keyReleaseEvent(QKeyEvent *e);

  void mouseMoveEvent(QMouseEvent *e);
  void mousePressEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);

private:
  QSGLWidgetPrivate *d;
};

#endif // QSGLWIDGET_H
