#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include "Objects.h"
#include <QElapsedTimer>

struct Data {
    std::string name;
    int x, y, z;
};

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
public:
    explicit MyOpenGLWidget(QWidget* parent = nullptr);
    void addObject(Object* obj,const std::string& name,const int& x,
        const int& y, const int z);
    void clearScene();
    void removeObj(Object* obj);
protected:
    void initializeGL() override; 
    void resizeGL(int w, int h) override; 
    void paintGL() override;

private:
    std::map<Object*,Data> objects; 
};
