#ifndef QSGLWIDGET_H
#define QSGLWIDGET_H

#include <QOpenGLWidget>

#include "Types.h"

class GLWidget : public QOpenGLWidget {
  Q_OBJECT
public:
  GLWidget(QWidget *parent = 0);
  ~GLWidget();

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
  void toggleFullScreen();

protected:
  SimpleGL::WindowPtr window { nullptr };
  vector<SimpleGL::LightPtr> lights;
  bool moving { false };
  QPoint referencePosition { 0, 0 };
  QPoint mousePosition { 0, 0 };
};

#endif // QSGLWIDGET_H
