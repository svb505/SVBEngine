#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include "Objects.h"
#include <QElapsedTimer>

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
public:
    explicit MyOpenGLWidget(QWidget* parent = nullptr);
    void addObject(Object* obj);     
    void clearScene();
    void removeObj(Object* obj);
protected:
    void initializeGL() override; 
    void resizeGL(int w, int h) override; 
    void paintGL() override;

private:
    std::vector<Object*> objects; 
};
