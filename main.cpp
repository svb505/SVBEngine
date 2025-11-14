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
    window.showMaximized();
    window.setWindowTitle("SVBEgine - OpenGL");
    window.show();

    Box* box = new Box();
    box->position = { 30, 0, 0};
    box->color = { 255, 0, 0 };
    box->setSize(20.0f, 20.0f);
    ogl->addObj(box, "box1", 30, 0, 0, 255.0f,0.0f,0.0f);

    Circle* circle = new Circle();
    circle->position = { 100, 0, 0 };
    circle->color = { 0, 255, 0 };
    circle->setRadius(6.0f);
    ogl->addObj(circle, "circle1", 190, 0, 0, 0.0f, 255.0f, 0.0f);

    return app.exec();
}
