#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();
};

#endif // MAINWINDOW_H
