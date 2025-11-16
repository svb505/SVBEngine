#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include "Objects.h"
#include <QElapsedTimer>

struct Data {
    Object* obj;
    int x, y, z;
    float r, g, b;
};
struct ProjectionParams {
    float left, right, top, bottom, zNear, zFar;
};
class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT;

public:
    ProjectionParams getProjectionParams() const;
    std::string mode = "2D";
    explicit MyOpenGLWidget(QWidget* parent = nullptr);
    void addObj(Object* obj, const std::string& name, const int& x,
        const int& y, const int z,const float r, const float g, const float b);
    void clearScene();
    void removeObj(const std::string& name);
    void setMode(const std::string& m);
    void startMove(const std::string& name, int targetX, int speed);
    std::map <std::string, Data> getObjects();
    void moveObj(const std::string& name, const int& x, const int& y, const int z);
    int getX(const std::string& name);
    int getY(const std::string& name);
    int getZ(const std::string& name);
protected:
    void initializeGL() override; 
    void resizeGL(int w,int h) override; 
    void paintGL() override;
private:
    std::map < std::string, Data > objects;
    QTimer* animTimer = nullptr;
    std::string animName;
    int animTargetX = 0;
    int animSpeed = 5;   
    float aspect = 1.0f;
    float left = 0.0f;
    float right = 0.0f;
    float top = 0.0f;
    float bottom = 0.0f;
    float zNear = 0.0f;
    float zFar = 0.0f;
private slots:
    void animateMove();
};
