#include "MainWindow.h"

#include "AssimpImporter.h"
#include "Instance.h"
#include "Mesh.h"
#include "Root.h"
#include "SceneNode.h"
#include "SubMesh.h"

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
  QString path = QFileDialog::getOpenFileName(this, tr("Open Mesh"), QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation), tr("SimpleGL Meshes (*.sglm)"));
  // return if open canceled
  if (path.isNull())
    return;
  // load mesh
  Root::instance()->load("MODEL", path.toStdString());
  // retrieve mesh
  MeshPtr mesh = Root::instance()->retrieveMesh("MODEL");
  if (mesh) {
    uint vertexCount = 0, indexCount = 0;
    for (SubMeshPtr subMesh: mesh->subMeshes()) {
      vertexCount += subMesh->vertexCount();
      indexCount += subMesh->indexCount();
    }
    lblVertexCount->setText(QString::number(vertexCount));
    lblTriangleCount->setText(QString::number(indexCount / 3));
  }
  // enable actions
  actionSave->setEnabled(true);
  actionSaveAs->setEnabled(true);
  actionClose->setEnabled(true);
}

void MainWindow::fileSave() {
  QString path = QFileDialog::getSaveFileName(this, tr("Save Mesh"), QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation), tr("SimpleGL Meshes (*.sglm)"));
  // return if open canceled
  if (path.isNull())
    return;
  // load mesh
  Root::instance()->save("MODEL", path.toStdString());
}

void MainWindow::fileSaveAs() {
  QString path = QFileDialog::getSaveFileName(this, tr("Save Mesh As"), QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation), tr("SimpleGL Meshes (*.sglm)"));
  // return if open canceled
  if (path.isNull())
    return;
  // load mesh
  Root::instance()->save("MODEL", path.toStdString());
}

void MainWindow::fileImport() {
  QString path = QFileDialog::getOpenFileName(this, tr("Import Mesh"), QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation), tr("All Files (*.*)"));
  // return if open canceled
  if (path.isNull())
    return;
  // load mesh
  AssimpImporter::import("MODEL", path.toStdString());
  // retrieve mesh
  MeshPtr mesh = Root::instance()->retrieveMesh("MODEL");
  if (mesh) {
    uint vertexCount = 0, indexCount = 0;
    for (SubMeshPtr subMesh: mesh->subMeshes()) {
      vertexCount += subMesh->vertexCount();
      indexCount += subMesh->indexCount();
    }
    lblVertexCount->setText(QString::number(vertexCount));
    lblTriangleCount->setText(QString::number(indexCount / 3));
  }
  // enable actions
  actionSave->setEnabled(true);
  actionSaveAs->setEnabled(true);
  actionClose->setEnabled(true);
}

void MainWindow::fileClose() {
  Root::instance()->destroyMesh("MODEL");
  // update vertex and face counts
  lblVertexCount->setText("");
  lblTriangleCount->setText("");
  // enable actions
  actionSave->setEnabled(false);
  actionSaveAs->setEnabled(false);
  actionClose->setEnabled(false);
}

void MainWindow::fileExit() {
  this->close();
}

void MainWindow::helpAbout() {
  QMessageBox::about(this, tr("About %1").arg(QCoreApplication::applicationName()),
                     QString("<center><b>%1 %2</b><br>Copyright (c) 2012 %3<br><a href=\'%4'>%4</a><br><br><b>Authors</b><br>Abdurrahman AVCI<br><a href='mailto:abdurrahmanavci@gmail.com'>abdurrahmanavci@gmail.com</a></center>")
                     .arg(QCoreApplication::applicationName())
                     .arg(QCoreApplication::applicationVersion())
                     .arg(QCoreApplication::organizationName())
                     .arg(QCoreApplication::organizationDomain()));
}
