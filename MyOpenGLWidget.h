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

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT;

public:
    std::string mode = "2D";
    explicit MyOpenGLWidget(QWidget* parent = nullptr);
    void addObj(Object* obj, const std::string& name, const int& x,
        const int& y, const int z,const float r, const float g, const float b);
    void clearScene();
    void removeObj(const std::string& name);
    void setMode(const std::string& m);
    void startMove(const std::string& name, int targetX, int speed);
    void moveObj(Object* obj, const std::string& name, const int& x, const int& y, const int z, float r, float g, float b);
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
private slots:
    void animateMove();
};
