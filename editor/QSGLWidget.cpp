#include "QSGLWidget.h"

#include "Window.h"
#include "Root.h"

class QSGLWidgetPrivate {
public:
  SimpleGL::Window *window { nullptr };
};

QSGLWidget::QSGLWidget(QWidget *parent) : QGLWidget(parent), d(new QSGLWidgetPrivate()) {
}

QSGLWidget::~QSGLWidget() {
  delete d;
}

SimpleGL::Window *QSGLWidget::sglWindow() const {
  return d->window;
}

void QSGLWidget::initializeGL() {
  // create window
  d->window = SimpleGL::Root::instance()->createWindow(width(), height());
  // emit signal
  emit initialized();
}

void QSGLWidget::resizeGL(int width, int height) {
  if (d->window)
    d->window->setSize(width, height);
}

void QSGLWidget::paintGL() {
  if (d->window)
    d->window->update();
}

void QSGLWidget::keyPressEvent(QKeyEvent *e) {
  emit keyPressed(e);
}

void QSGLWidget::keyReleaseEvent(QKeyEvent *e) {
  emit keyReleased(e);
}

void QSGLWidget::mouseMoveEvent(QMouseEvent *e) {
  emit mouseMoved(e);
}

void QSGLWidget::mousePressEvent(QMouseEvent *e) {
  emit mousePressed(e);
}

void QSGLWidget::mouseReleaseEvent(QMouseEvent *e) {
  emit mouseReleased(e);
}

void QSGLWidget::wheelEvent(QWheelEvent *e) {
  emit wheelMoved(e);
}
