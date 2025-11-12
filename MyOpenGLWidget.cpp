#include "MyOpenGLWidget.h"
#include <QTimer>
#include <iostream>
#include <QDebug>
#include <map>


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
    glOrtho(-100,100,-100,100,-100,100);

    glMatrixMode(GL_MODELVIEW);
}
void MyOpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    for (auto& [name, data] : objects) {
        if (data.obj) {
            qDebug() << "Render:" << QString::fromStdString(name)
                << "x=" << data.x;
            glPushMatrix();
            glTranslatef(data.x, data.y, data.z);
            glColor3f(data.r, data.g, data.b);
            data.obj->render();
            glPopMatrix();
        }
    }
}
void MyOpenGLWidget::addObject(Object* obj,const std::string& name,const int& x,
    const int& y,const int z,const int r,const int g, const  int b) {
    objects[name] = {obj,x,y,z,r,g,b};
    update(); 
}
void MyOpenGLWidget::clearScene() {
    objects.clear();
    update();
}
void MyOpenGLWidget::removeObj(const std::string& name) {
    auto it = objects.find(name);
    if (it != objects.end()) {
        objects.erase(it);
        update();
    }
}
