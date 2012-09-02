#ifndef QSGLWIDGET_H
#define QSGLWIDGET_H

#include <QGLWidget>

namespace SimpleGL {
  class Camera;
  class Instance;
  class SceneNode;
  class Window;
}

class GLWidget : public QGLWidget {
  Q_OBJECT
public:
  GLWidget(QWidget *parent = 0);
  ~GLWidget();

  SimpleGL::Instance *instance() const;

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

protected:
  SimpleGL::Window *window { nullptr };
  SimpleGL::Camera *camera { nullptr };
  SimpleGL::SceneNode *cameraNode { nullptr };
  SimpleGL::Instance *_instance { nullptr };
  QPoint mousePosition { 0, 0 };
};

#endif // QSGLWIDGET_H
