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
#include <QElapsedTimer>
#include "Camera.h"
#include <QPainter>
#include <QMenu>
#include "Logger.h"


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
MyOpenGLWidget::MyOpenGLWidget(QWidget* parent) : QOpenGLWidget(parent) {
    LOG_INFO("[RENDER] OpenGLWidget created");
    animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, this, &MyOpenGLWidget::animateMove);
    fpsTimer.start();
    initHUD();
}
ProjectionParams MyOpenGLWidget::getProjectionParams() const {
    return { left, right, top, bottom, zNear, zFar };
}
void MyOpenGLWidget::initializeGL() {
    LOG_INFO("[RENDER] OpenGL Initializated");
    initializeOpenGLFunctions();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
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
        fps = fpsFrames;
        fpsFrames = 0;
        fpsTimer.restart();
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
    glLoadIdentity();

    glTranslatef(cam.camX, cam.camY, cam.negativeCamDistance);
    glRotatef(cam.camPitch, 1, 0, 0);
    glRotatef(cam.camYaw, 0, 1, 0);

    drawGridOpenGL(100.0f, 10);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setFont(QFont("Consolas", 11));
    painter.setPen(Qt::white);
    drawGridText(painter, 100.0f, 10);
    painter.end();

    for (auto& [name, data] : objects) {
        if (data.obj)
            data.obj->render();
    }
    if (hud)
        hud->drawHud(this, &cam, &gui);
  
    update();
}
MyOpenGLWidget::~MyOpenGLWidget() {
    LOG_INFO("[RENDER] OpenGL destroyed");
    delete hud;
}
void MyOpenGLWidget::initHUD() {
    if (!hud)
        LOG_INFO("[HUD] HUD Initializated");
        hud = new HUD(this, &cam, &gui);
}
void MyOpenGLWidget::setBackground(std::array<float, 3> color) {
    LOG_INFO("[SCENE] Background color changed");
    for (int i = 0; i < 3; i++) {
        backgroundColor[i] = color[i];
    }
}
void MyOpenGLWidget::drawGridOpenGL(float spacing, int count) {
    glPushMatrix();
    glLineWidth(1.0f);
    glBegin(GL_LINES);

    if (mode == "2D") {
        for (int i = -count; i <= count; ++i) {
            if (i == 0)
                glColor3f(1.0f, 0.0f, 0.0f);
            else
                glColor3f(0.5f, 0.5f, 0.5f);

            float pos = i * spacing;
            glVertex3f(pos, -count * spacing, 0);
            glVertex3f(pos, count * spacing, 0);
            glVertex3f(-count * spacing, pos, 0);
            glVertex3f(count * spacing, pos, 0);
        }
    }
    else if (mode == "3D") {
        for (int i = -count; i <= count; ++i) {
            float pos = i * spacing;
            glColor3f(1.0f, 1.0f, 1.0f);
            glVertex3f(-count * spacing, 0, pos);
            glVertex3f(count * spacing, 0, pos);

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
void MyOpenGLWidget::drawGridText(QPainter& painter, float spacing, int count) {
    if (mode != "2D") return;

    for (int x = int(left); x < int(right); x += int(spacing))
        drawText(painter, x, 0, QString::number(x));

    for (int y = int(bottom); y < int(top); y += int(spacing))
        if (y != 3) {
            drawText(painter, 0, y, QString::number(y));
        }
}
void MyOpenGLWidget::drawText(QPainter& painter, int x, int y, QString text) {
    QPointF p = worldToScreen(x + 0.1, y + 0.2, this->width(), this->height());
    painter.drawText(p, text);
}
QPointF MyOpenGLWidget::worldToScreen(float x, float y, int widgetWidth, int widgetHeight) const {
    float sx = (x - left) / (right - left) * widgetWidth;
    float sy = widgetHeight - (y - bottom) / (top - bottom) * widgetHeight;
    return QPointF(sx, sy);
}
void MyOpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
    if (!cam.cameraFix) {
        cam.mouseWheel(event);
        update();
    }
}
void MyOpenGLWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::MiddleButton) {
        gui.addContexMenu(event, this, this->window());
    }
    if (!cam.cameraFix) {
        cam.changeLastMouse(event);
    }
    update();
}
void MyOpenGLWidget::addObj(Object* obj, const std::string& name, const std::string& type,
    float x, float y, float z, float r, float g, float b)
{
    LOG_INFO(std::format("[SCENE] Object '{}' added",name));
    objects[name] = { obj, type, x, y, z, r, g, b };
    update();
}
void MyOpenGLWidget::removeObj(const std::string& name) {
    LOG_INFO(std::format("[SCENE] Object with name '{}' has been removed", name));
    objects.erase(name);
    update();
}
void MyOpenGLWidget::clearScene() {
    LOG_INFO("[SCENE] Scene cleared");
    objects.clear();
    update();
}
void MyOpenGLWidget::setMode(const std::string& m) {
    LOG_INFO(std::format("[SCENE] Current mode: {}",m));
    mode = m;
    update();
}
void MyOpenGLWidget::moveObj(const std::string& name, const float& x, const float& y, const float z) {
    auto it = objects.find(name);
    if (it == objects.end()) {
        LOG_INFO(std::format("[SCENE] Object not found: {}",name));
        return;
    }
    Object* obj = it->second.obj;
    float r = it->second.r;
    float g = it->second.g;
    float b = it->second.b;
    obj->position = { x, y, z };
    obj->color = { r, g, b };
    it->second.x = x;
    it->second.y = y;
    it->second.z = z;
    update();
}
void MyOpenGLWidget::startMove(const std::string& name, int targetX, int speed) {
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
int MyOpenGLWidget::getX(const std::string& name) { return objects[name].x; }
int MyOpenGLWidget::getY(const std::string& name) { return objects[name].y; }
int MyOpenGLWidget::getZ(const std::string& name) { return objects[name].z; }
int MyOpenGLWidget::getTurnX(const std::string& name) { return objects[name].obj->turnX; }
int MyOpenGLWidget::getTurnY(const std::string& name) { return objects[name].obj->turnY; }
int MyOpenGLWidget::getTurnZ(const std::string& name) { return objects[name].obj->turnZ; }
std::string MyOpenGLWidget::getType(const std::string& name) { return objects[name].type; }
std::map<std::string, Data> MyOpenGLWidget::getObjects() const
{
    return objects; 
}

std::vector<float> MyOpenGLWidget::getColors(const std::string& name) {
    return { objects[name].r, objects[name].g, objects[name].b };
}
void MyOpenGLWidget::changeObj(const std::string& name, float x, float y, float z, float colors[], int turnX,
    int turnY, int turnZ)
{
    LOG_INFO(std::format("[SCENE] Object with name '{}' has been changed", name));
    auto it = objects.find(name);
    if (it == objects.end()) {
        LOG_ERROR(std::format("Object with name '{}' not found!", name));
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
