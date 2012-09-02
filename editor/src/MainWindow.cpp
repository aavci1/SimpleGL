#include "MainWindow.h"

#include "Instance.h"
#include "Mesh.h"
#include "Root.h"
#include "SceneNode.h"

#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

using namespace SimpleGL;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setupUi(this);
  // connect menu actions
  connect(actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
  connect(actionSave, SIGNAL(triggered()), this, SLOT(fileSave()));
  connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
  connect(actionImport, SIGNAL(triggered()), this, SLOT(fileImport()));
  connect(actionClose, SIGNAL(triggered()), this, SLOT(fileClose()));
  connect(actionExit, SIGNAL(triggered()), this, SLOT(fileExit()));
  connect(actionAbout, SIGNAL(triggered()), this, SLOT(helpAbout()));
  // create timer
  timer = new QTimer(this);
  // set timer interval
  timer->setInterval(10);
  // connect update
  connect(timer, SIGNAL(timeout()), widget, SLOT(updateGL()));
  // start the timer
  timer->start();
}

MainWindow::~MainWindow() {
}

void MainWindow::fileOpen() {
//  if (!confirmClose())
//    return;
  QString path = QFileDialog::getOpenFileName(this, tr("Open Mesh"), QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation), tr("SimpleGL Meshes (*.sglm)"));
  // return if open canceled
  if (path.isNull())
    return;
  // load mesh
  Root::instance()->load("MODEL", path.toStdString());
  // create an instance
  Root::instance()->rootSceneNode()->createChildSceneNode()->attachObject(Root::instance()->createInstance("MODEL", "Markus"));
}

void MainWindow::fileSave() {

}

void MainWindow::fileSaveAs() {

}

void MainWindow::fileImport() {

}

void MainWindow::fileClose() {

}

void MainWindow::fileExit() {

}

void MainWindow::helpAbout() {
  QMessageBox::about(this, tr("About %1").arg(QCoreApplication::applicationName()),
                     QString("<center><b>%1 %2</b><br>Copyright (c) 2012 %3<br><a href=\'%4'>%4</a><br><br><b>Authors</b><br>Abdurrahman AVCI<br><a href='mailto:abdurrahmanavci@gmail.com'>abdurrahmanavci@gmail.com</a></center>")
                     .arg(QCoreApplication::applicationName())
                     .arg(QCoreApplication::applicationVersion())
                     .arg(QCoreApplication::organizationName())
                     .arg(QCoreApplication::organizationDomain()));
}
