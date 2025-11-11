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
    box->position = { -2, 0, 0 };
    box->color = { 1, 5, 0 }; 
    ogl->addObject(box);

    Circle* circle = new Circle();
    circle->position = { -6, 0, 0 };
    circle->color = { 0, 1, 0 }; 
    ogl->addObject(circle);

    return app.exec();
}
