#include "MyOpenGLWidget.h"

MyOpenGLWidget::MyOpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent) {
}

void MyOpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_BLEND);                 // включаем прозрачность
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.1f, 0.1f, 0.0f, 1.0f); // цвет фона
}

void MyOpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Ортографическая проекция (2D)
    float aspect = float(w) / float(h);
    glOrtho(-aspect * 5, aspect * 5, -5, 5, -10, 10);

    glMatrixMode(GL_MODELVIEW);
}

void MyOpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Отрисовываем все объекты
    for (auto obj : objects)
        obj->render();
}

void MyOpenGLWidget::addObject(Object* obj) {
    objects.push_back(obj);
    update(); // перерисовать экран
}
