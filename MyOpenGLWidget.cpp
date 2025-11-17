#include "MyOpenGLWidget.h"
#include <QTimer>
#include <iostream>
#include <QDebug>
#include <map>
#include <qDebug>
#include <QPainter>
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
    animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, this, &MyOpenGLWidget::animateMove);
}
ProjectionParams MyOpenGLWidget::getProjectionParams() const {
    return { left, right, top, bottom, zNear, zFar };
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
        // left/right/top/bottom могут быть какими хочешь
        left = -400 * aspect;
        right = 400 * aspect;
        bottom = -200;
        top = 200;

        // НО near/far должны быть ПОЛОЖИТЕЛЬНЫМИ!
        zNear = 0.1f;
        zFar = 1000.0f;

        glOrtho(left, right, bottom, top, zNear, zFar);
    }
    else { // 3D
        perspective(60.0f, aspect, 0.1f, 1000.0f);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Камера назад
    glTranslatef(0, 0, -400);
}

void MyOpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Камера
    glTranslatef(0, 0, -400);

    for (auto& [name, data] : objects) {
        if (data.obj) {
            data.obj->render(); // уже сам делает translate/scale
        }
    }
}

void MyOpenGLWidget::addObj(Object* obj,const std::string& name,const float& x,const float& y,const float z,
    float r, float g, float b) {
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
std::map <std::string, Data> MyOpenGLWidget::getObjects() {
    return objects;
}
void MyOpenGLWidget::setMode(const std::string& m) {
    mode = m;
    update();
}
void MyOpenGLWidget::moveObj(const std::string& name, const float& x, const float& y, const float z)
{
    auto it = objects.find(name);
    if (it == objects.end()) {
        qDebug() << "Object not found:" << QString::fromStdString(name);
        return;
    }
    Object* obj = it->second.obj;
    float r = it->second.r;
    float g = it->second.g;
    float b = it->second.b;
    obj->position = { float(x), float(y), float(z) };
    obj->color = { r, g, b };
    it->second.x = x;
    it->second.y = y;
    it->second.z = z;

    update();
};
int MyOpenGLWidget::getX(const std::string& name) {
    return objects[name].x;
};
int MyOpenGLWidget::getY(const std::string& name) {
    return objects[name].y;
};
int MyOpenGLWidget::getZ(const std::string& name) {
    return objects[name].z;
};
void MyOpenGLWidget::startMove(const std::string& name, int targetX, int speed)
{
    animName = name;
    animTargetX = targetX;
    animSpeed = speed;

    animTimer->start(16); 
}
void MyOpenGLWidget::animateMove() {
    auto& data = objects[animName];

    if (data.x < animTargetX)
        data.x += animSpeed;
    else {
        animTimer->stop();
        return;
    }

    update();
}

