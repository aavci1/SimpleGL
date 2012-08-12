#include "MainWindow.h"

#include "Root.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setupUi(this);
  // initialize SimpleGL
  SimpleGL::Root::initialize();
}

MainWindow::~MainWindow() {
  // deinitialize SimpleGL
  SimpleGL::Root::destroy();
}
