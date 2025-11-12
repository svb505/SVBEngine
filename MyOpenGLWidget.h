#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include "Objects.h"
#include <QElapsedTimer>

struct Data {
    Object* obj;
    int x, y, z;
    int r, g, b;
};

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
public:
    explicit MyOpenGLWidget(QWidget* parent = nullptr);
    void addObject(Object* obj, const std::string& name, const int& x,
        const int& y, const int z,const int r, const int g, const  int b);
    void clearScene();
    void removeObj(const std::string& name);
protected:
    void initializeGL() override; 
    void resizeGL(int w, int h) override; 
    void paintGL() override;

private:
    std::map < std::string, Data > objects;
};
