#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"

namespace SimpleGL {
  class Camera;
  class SceneNode;
  class Window;
}

class MainWindow : public QMainWindow, private Ui::MainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

public slots:
  void initialized();

  void keyPressed(QKeyEvent *e);
  void keyReleased(QKeyEvent *e);

  void mouseMoved(QMouseEvent *e);
  void mousePressed(QMouseEvent *e);
  void mouseReleased(QMouseEvent *e);

  void wheelMoved(QWheelEvent *e);

private:
  SimpleGL::Camera *camera { nullptr };
  SimpleGL::SceneNode *cameraNode { nullptr };
  QPoint mousePosition { 0, 0 };
};

#endif // MAINWINDOW_H
