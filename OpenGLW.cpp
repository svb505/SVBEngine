#include "OpenGLW.h"
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
#include "Grid.h"

OpenGLW::OpenGLW(QWidget* parent) : QOpenGLWidget(parent) {
    LOG_INFO("[RENDER] OpenGLWidget created");
    objects.reserve(100);
    animSys = new AnimationSystem(this, this);
    fpsTimer.start();
    initHUD();
    initSceneText();
}
OpenGLW::~OpenGLW() {
    LOG_INFO("[RENDER] OpenGL destroyed");
    delete hud;
    delete text;
}
ProjectionParams OpenGLW::getProjectionParams() const { return { left, right, top, bottom, zNear, zFar };}
void OpenGLW::initializeGL() {
    LOG_INFO("[RENDER] OpenGL Initializated");
    initializeOpenGLFunctions();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    fpsFrames = 0;
}
void OpenGLW::resizeGL(int w, int h) {
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
    else cam.perspective(60.0f, aspect, 0.1f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -400);
}
void OpenGLW::paintGL() {
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

    drawGridOpenGL(100.0f, 10,mode);

    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setFont(QFont("Consolas", 11));
    painter.setPen(Qt::white);

    for (auto& t : texts){
        if (mode == "2D") text->drawText(painter, t.x, t.y, t.text, this);
        else text->drawText3D(t.x, t.y, t.z, t.text, t.r, t.g, t.b);
    }

    if (mode == "2D") text->drawGridText(painter, 100.0f, 10,this);
    else text->draw3DGridText(100.0f,this);

    painter.end();

    for (auto& [name, data] : objects) {
        if (data.x >= left && data.x <= right && data.y >= bottom && data.y <= top && data.z <= zFar)  
            if (data.obj) data.obj->render();
    }
    if (hud) hud->drawHud(this, &cam, &gui);
  
    update();
}
void OpenGLW::initHUD() {
    if (!hud)
        LOG_INFO("[HUD] HUD Initializated");
        hud = new HUD(this, &cam, &gui);
}
void OpenGLW::initSceneText() {
    if (!text) LOG_INFO("[SceneText] SceneText Initializated");
    text = new SceneText();
}
void OpenGLW::setBackground(std::array<float, 3> color) {
    LOG_INFO("[SCENE] Background color changed");
    for (int i = 0; i < 3; i++) backgroundColor[i] = color[i];
}
void OpenGLW::mouseMoveEvent(QMouseEvent* event) {
    if (!cam.cameraFix) {
        cam.mouseWheel(event);
        update();
    }
}
void OpenGLW::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::MiddleButton) gui.addContexMenu(event, this, this->window());
    if (!cam.cameraFix) cam.changeLastMouse(event);

    update();
}
void OpenGLW::addObj(Object* obj, const std::string& name, const std::string& type,
    float x, float y, float z, float r, float g, float b,std::string parent){
    LOG_INFO(std::format("[SCENE] Object '{}' added",name));
    objects[name] = { obj, type, x, y, z, r, g, b, parent};
    update();
}
void OpenGLW::removeObj(const std::string& name){
    auto it = objects.find(name);
    if (it == objects.end()) return;

    Object* target = it->second.obj;

    LOG_INFO(std::format("[SCENE] Object with name '{}' has been removed",name));

    for (auto iter = objects.begin(); iter != objects.end(); ){
        if (iter->second.parent == name) iter = objects.erase(iter);  
        else ++iter;
    }

    objects.erase(it);

    update();
}
void OpenGLW::clearScene() {
    LOG_INFO("[SCENE] Scene cleared");
    objects.clear();
    update();
}
void OpenGLW::setMode(const std::string& m) {
    LOG_INFO(std::format("[SCENE] Current mode: {}",m));
    mode = m;
    update();
}
void OpenGLW::changeObj(const std::string& name, float x, float y, float z, float colors[], int turnX,
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
void OpenGLW::moveObj(const std::string& name, const float& x, const float& y, const float z) {
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
int OpenGLW::getX(const std::string& name) { return objects[name].x; }
int OpenGLW::getY(const std::string& name) { return objects[name].y; }
int OpenGLW::getZ(const std::string& name) { return objects[name].z; }
int OpenGLW::getTurnX(const std::string& name) { return objects[name].obj->turnX; }
int OpenGLW::getTurnY(const std::string& name) { return objects[name].obj->turnY; }
int OpenGLW::getTurnZ(const std::string& name) { return objects[name].obj->turnZ; }
std::string OpenGLW::getType(const std::string& name) { return objects[name].type; }
std::unordered_map<std::string, Data> OpenGLW::getObjects() const {return objects; }
std::vector<float> OpenGLW::getColors(const std::string& name) {return { objects[name].r, objects[name].g, objects[name].b };}
