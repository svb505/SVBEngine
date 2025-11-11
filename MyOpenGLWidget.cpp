#include "MyOpenGLWidget.h"
#include <QTimer>
#include <iostream>
#include <QDebug>

MyOpenGLWidget::MyOpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
}


void MyOpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_BLEND);                 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.1f, 0.1f, 0.0f, 1.0f); 
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
    for (auto& pair : objects)
        pair.first->render();
}
void MyOpenGLWidget::addObject(Object* obj,const std::string& name,const int& x,
    const int& y,const int z) {
    objects[obj] = {name,x,y,z};
    update(); 
}
void MyOpenGLWidget::clearScene() {
    for (auto& pair : objects) delete pair.first;
    objects.clear();
    update();
}
void MyOpenGLWidget::removeObj(Object* obj) {
    auto it = objects.find(obj);
    if (it != objects.end()) {
        delete it->first;
        objects.erase(obj);
        update();
    }
}
