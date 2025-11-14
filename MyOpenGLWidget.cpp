#include "MyOpenGLWidget.h"
#include <QTimer>
#include <iostream>
#include <QDebug>
#include <map>

void perspective(float fov, float aspect, float zNear, float zFar) {
    float f = 1.0f / tanf(fov * 0.5f * M_PI / 180.0f);

    float mat[16] = {
        f / aspect, 0, 0, 0,
        0, f,        0, 0,
        0, 0,  (zFar + zNear) / (zNear - zFar), -1,
        0, 0, (2 * zFar * zNear) / (zNear - zFar), 0
    };

    glMultMatrixf(mat);
}

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
    float aspect = float(w) / float(h);
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (mode == "2D") {
        glOrtho(-200 * aspect, 200 * aspect, -100 * aspect, 100 * aspect, -100, 100);
    }
    else {
        perspective(60.0f, aspect, 0.1f, 1000.0f);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -400);
}
void MyOpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    for (auto& [name, data] : objects) {
        if (data.obj) {
            glPushMatrix();
            glTranslatef(data.x, data.y, data.z);
            glColor3f(data.r, data.g, data.b);
            data.obj->render();
            glPopMatrix();
        }
    }
}
void MyOpenGLWidget::addObj(Object* obj,const std::string& name,const int& x,
    const int& y,const int z,float r, float g, float b) {
    r /= 255.0f,g /= 255.0f,b /= 255.0f;
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
void MyOpenGLWidget::setMode(const std::string& m) {
    mode = m;
    update();
}