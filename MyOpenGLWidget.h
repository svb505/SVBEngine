#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include "Objects.h"

/*
 * КЛАСС MyOpenGLWidget
 * --------------------
 * Это основное окно для отрисовки.
 * Оно управляет контекстом OpenGL и списком объектов.
 */
class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
public:
    explicit MyOpenGLWidget(QWidget* parent = nullptr);

    void addObject(Object* obj); // добавить объект в сцену

protected:
    void initializeGL() override; // инициализация OpenGL
    void resizeGL(int w, int h) override; // изменение размера окна
    void paintGL() override; // отрисовка каждого кадра

private:
    std::vector<Object*> objects; // все объекты сцены
};
