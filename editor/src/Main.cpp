#include "MainWindow.h"

#include <QApplication>

int main(int argc, char **argv) {
  // set company and product info
  QCoreApplication::setOrganizationName("Aavci Software");
  QCoreApplication::setOrganizationDomain("http://www.aavci.com");
  QCoreApplication::setApplicationName("SimpleGL Editor");
  QCoreApplication::setApplicationVersion("2012");
  // create application
  QApplication app(argc, argv);

  QSurfaceFormat format;
  format.setVersion(4, 5);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  QSurfaceFormat::setDefaultFormat(format);

  // create main window
  MainWindow main;
  // show main window
  main.show();
  // start event loop
  return app.exec();
}
