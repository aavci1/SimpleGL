#include "MainWindow.h"

#include "AssimpImporter.h"
#include "FbxImporter.h"
#include "Instance.h"
#include "Mesh.h"
#include "Model.h"
#include "Root.h"
#include "SceneNode.h"

#include <QDesktopServices>
#include <QDesktopWidget>
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
  connect(actionFullScreen, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));
  connect(widget, SIGNAL(toggleFullScreen()), this, SLOT(toggleFullScreen()));
  connect(actionAbout, SIGNAL(triggered()), this, SLOT(helpAbout()));
  // schedule update
  QTimer::singleShot(1, this, SLOT(updateView()));
}

MainWindow::~MainWindow() {
}

void MainWindow::fileOpen() {
  QString path = QFileDialog::getOpenFileName(this, tr("Open Model"), QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation), tr("SimpleGL Models (*.model)"));
  // return if open canceled
  if (path.isNull())
    return;
  // load model
  Root::instance()->load("MODEL", path.toStdString());
  // retrieve model
  ModelPtr model = Root::instance()->retrieveModel("MODEL");
  if (model) {
    vertexCount = 0;
    indexCount = 0;
    for (MeshPtr mesh: model->meshes()) {
      vertexCount += mesh->vertexCount();
      indexCount += mesh->indexCount();
    }
    // update status message
    sbMain->showMessage(tr("FPS: %1 | Vertices: %2 | Indices: %3").arg(fps).arg(vertexCount).arg(indexCount));
  }
  // enable actions
  actionSave->setEnabled(true);
  actionSaveAs->setEnabled(true);
  actionClose->setEnabled(true);
}

void MainWindow::fileSave() {
  QString path = QFileDialog::getSaveFileName(this, tr("Save Model"), QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation), tr("SimpleGL Models (*.model)"));
  // return if open canceled
  if (path.isNull())
    return;
  // load model
  Root::instance()->save("MODEL", path.toStdString());
}

void MainWindow::fileSaveAs() {
  QString path = QFileDialog::getSaveFileName(this, tr("Save Model As"), QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation), tr("SimpleGL Models (*.model)"));
  // return if open canceled
  if (path.isNull())
    return;
  // load model
  Root::instance()->save("MODEL", path.toStdString());
}

void MainWindow::fileImport() {
  QString path = QFileDialog::getOpenFileName(this, tr("Import Model"), QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation), tr("FBX Files (*.fbx);;All Files (*.*)"));
  // return if open canceled
  if (path.isNull())
    return;
  // load model
  AssimpImporter::import("MODEL", path.toStdString());
  // retrieve model
  ModelPtr model = Root::instance()->retrieveModel("MODEL");
  if (model) {
    vertexCount = 0;
    indexCount = 0;
    for (MeshPtr mesh: model->meshes()) {
      vertexCount += mesh->vertexCount();
      indexCount += mesh->indexCount();
    }
    // update status message
    sbMain->showMessage(tr("FPS: %1 | Vertices: %2 | Indices: %3").arg(fps).arg(vertexCount).arg(indexCount));
  }
  // enable actions
  actionSave->setEnabled(true);
  actionSaveAs->setEnabled(true);
  actionClose->setEnabled(true);
}

void MainWindow::fileClose() {
  Root::instance()->removeModel("MODEL");
  // update status message
  sbMain->showMessage(QString("Ready"));
  // enable actions
  actionSave->setEnabled(false);
  actionSaveAs->setEnabled(false);
  actionClose->setEnabled(false);
}

void MainWindow::fileExit() {
  this->close();
}

void MainWindow::toggleFullScreen() {
  static bool fullScreen = false;
  fullScreen = !fullScreen;
  if (fullScreen) {
    mbMain->hide();
    sbMain->hide();
    setWindowState(Qt::WindowFullScreen);
  } else {
    mbMain->show();
    sbMain->show();
    setWindowState(Qt::WindowNoState);
  }
}

void MainWindow::helpAbout() {
  QMessageBox::about(this, tr("About %1").arg(QCoreApplication::applicationName()),
                     QString("<center><b>%1 %2</b><br>Copyright (c) 2012 %3<br><a href=\'%4'>%4</a><br><br><b>Authors</b><br>Abdurrahman AVCI<br><a href='mailto:abdurrahmanavci@gmail.com'>abdurrahmanavci@gmail.com</a></center>")
                     .arg(QCoreApplication::applicationName())
                     .arg(QCoreApplication::applicationVersion())
                     .arg(QCoreApplication::organizationName())
                     .arg(QCoreApplication::organizationDomain()));
}

void MainWindow::updateView() {
  // update widget
  widget->updateGL();
  // update status message
  fps = Root::instance()->fps();
  sbMain->showMessage(tr("FPS: %1 | Vertices: %2 | Indices: %3").arg(fps).arg(vertexCount).arg(indexCount));
  // schedule next update
  QTimer::singleShot(1, this, SLOT(updateView()));
}
