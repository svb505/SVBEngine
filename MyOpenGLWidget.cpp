#include "MyOpenGLWidget.h"
#include <QTimer>
#include <iostream>
#include <map>
#include <qDebug>
#ifdef _WIN32
#include <windows.h>
#endif
#include <QWheelEvent>
#include <QMouseEvent>
#include <GL/gl.h>
#include <GL/glu.h>   
#include <QELapsedTimer>
#include "Camera.h"

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

    fpsTimer.start();   
}
ProjectionParams MyOpenGLWidget::getProjectionParams() const {
    return { left, right, top, bottom, zNear, zFar };
}
void MyOpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_BLEND);                 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.1f, 0.1f, 0.0f, 1.0f); 
    fpsTimer.start();    
    fpsFrames = 0;
}
void MyOpenGLWidget::resizeGL(int w, int h) {
    float aspect = float(w) / float(h);

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (mode == "2D") {
        float size = 400;

        left = -size;
        right = size;
        bottom = -size / aspect;
        top = size / aspect;
        zNear = 0.1f;
        zFar = 1000.0f;


        glOrtho(left, right, bottom, top, zNear, zFar);
    }
    else {
        perspective(60.0f, aspect, 0.1f, 1000.0f);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0, 0, -400);
}
void MyOpenGLWidget::paintGL() {
    fpsFrames++;
    if (fpsTimer.elapsed() >= 1000) {   
        int fps = fpsFrames;
        fpsFrames = 0;
        fpsTimer.restart();

        if (parentWidget()) {
            parentWidget()->setWindowTitle(QString("SVBEngine - Render: OpenGL - FPS: %1").arg(fps));
        }
    }
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(cam.camX, cam.camY, cam.negativeCamDistance);
    glRotatef(cam.camPitch, 1, 0, 0);
    glRotatef(cam.camYaw, 0, 1, 0);

    drawGrid(100.0f, 10);
    for (auto& [name, data] : objects) {
        if (data.obj) {
            data.obj->render();
        }
    }
    update();
}
void MyOpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    cam.mouseWheel(event);
    update();
}
void MyOpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    cam.changeLastMouse(event);
    update();
}
void MyOpenGLWidget::wheelEvent(QWheelEvent* event)
{
    cam.wheelEvent(event);

    update();
}
void MyOpenGLWidget::addObj(Object* obj,const std::string& name, const std::string& type,const float& x,const float& y,const float z,
    float r, float g, float b) {
    objects[name] = {obj,type,x,y,z,r,g,b};
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
std::vector<float> MyOpenGLWidget::getColors(const std::string& name) {
    std::vector<float> vect = { objects[name].r,objects[name].g, objects[name].b };
    return vect;
}
void MyOpenGLWidget::changeObj(const std::string& name, float x, float y, float z, float colors[],int turnX,
    int turnY,int turnZ)
{
    auto it = objects.find(name);
    if (it == objects.end()) {
        qDebug() << "Object not found!";
        return;
    }
    it->second.obj->position = { x, y, z };
    it->second.obj->color = { colors[0], colors[1], colors[2] };
    it->second.obj->turnX = turnX;
    it->second.obj->turnY = turnY;
    it->second.obj->turnZ = turnZ;

    it->second.x = x;
    it->second.y = y;
    it->second.z = z;
    it->second.r = colors[0];
    it->second.g = colors[1];
    it->second.b = colors[2];
    
    update();
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
int MyOpenGLWidget::getTurnX(std::string& name) { return objects[name].obj->turnX; }
int MyOpenGLWidget::getTurnY(std::string& name) { return objects[name].obj->turnY; }
int MyOpenGLWidget::getTurnZ(std::string& name) { return objects[name].obj->turnZ; }
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
void MyOpenGLWidget::drawGrid(float spacing, int count) {
    glPushMatrix();

    glColor3f(0.5f, 0.5f, 0.5f);
    glLineWidth(1.0f);

    glBegin(GL_LINES);

    if (mode == "2D") {
        for (int i = -count; i <= count; ++i) {
            float pos = i * spacing;

            // Vertical lines
            glVertex3f(pos, -count * spacing, 0);
            glVertex3f(pos, count * spacing, 0);

            // Horizontal lines
            glVertex3f(-count * spacing, pos, 0);
            glVertex3f(count * spacing, pos, 0);
        }
    }
    else if (mode == "3D") {
        for (int i = -count; i <= count; ++i) {
            float pos = i * spacing;
            //X
            glColor3f(1.0f, 1.0f, 1.0f);
            glVertex3f(-count * spacing, 0, pos);
            glVertex3f(count * spacing, 0, pos);
            //Z
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(pos, 0, -count * spacing);
            glVertex3f(pos, 0, count * spacing);
        }

        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0, -count * spacing, 0);
        glVertex3f(0, count * spacing, 0);
    }

    glEnd();
    glPopMatrix();
}
std::string MyOpenGLWidget::getType(const std::string& name) { return objects[name].type; }
