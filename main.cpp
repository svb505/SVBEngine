#include <QApplication>
#include <QMainWindow>
#include "MyOpenGLWidget.h"
#include "Objects.h"
#include <iostream>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QMainWindow window;
    auto* ogl = new MyOpenGLWidget();
    window.setCentralWidget(ogl);
    window.resize(800, 600);
    window.show();

    Box* box = new Box();
    box->position = { -6, 0, 0 };
    box->color = { 255, 0, 0 };
    box->width = 20.0f;
    box->height = 20.0f;
    ogl->addObject(box, "box1", -3, 0, 0, 255, 0, 0);

    Circle* circle = new Circle();
    circle->position = { -2, 0, 0 };
    circle->color = { 0, 255, 0 };
    ogl->addObject(circle, "circle1", -2, 0, 0, 0, 255, 0);

    Circle* circle1 = new Circle();
    circle1->position = { 2, 0, 0 };
    circle1->color = { 0, 0, 255 };
    ogl->addObject(circle1, "circle2", 2, 0, 0, 0, 0, 255);

    return app.exec();
}
