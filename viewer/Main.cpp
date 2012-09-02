#include <QApplication>

#include "MainWindow.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  // create main window
  MainWindow main;
  // show main window
  main.show();
  // start event loop
  return app.exec();
}
