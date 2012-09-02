#include <QApplication>

#include "MainWindow.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  // set company and product info
  QCoreApplication::setOrganizationName("Aavci Software");
  QCoreApplication::setOrganizationDomain("http://www.aavci.com");
  QCoreApplication::setApplicationName("SimpleGL Editor");
  QCoreApplication::setApplicationVersion("2012");
  // create main window
  MainWindow main;
  // show main window
  main.show();
  // start event loop
  return app.exec();
}
