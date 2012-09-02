#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"

class QTimer;

class MainWindow : public QMainWindow, private Ui::MainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

public slots:
  void fileOpen();
  void fileSave();
  void fileSaveAs();
  void fileImport();
  void fileClose();
  void fileExit();
  void helpAbout();

private:
  QTimer *timer;
};

#endif // MAINWINDOW_H
