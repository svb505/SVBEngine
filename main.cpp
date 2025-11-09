#include <QApplication>
#include <QMainWindow>
#include "MyOpenGLWidget.h"
#include "Objects.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QMainWindow window;
    auto* ogl = new MyOpenGLWidget();
    window.setCentralWidget(ogl);
    window.resize(800, 600);
    window.show();

    // 🔸 Добавляем квадрат
    Box* box = new Box();
    box->position = { -2, 0, 0 };
    box->color = { 1, 1, 0 }; 
    ogl->addObject(box);

    // 🔸 Добавляем круг
    Circle* circle = new Circle();
    circle->position = { 2, 0, 0 };
    circle->color = { 0, 1, 0 }; // зелёный
    ogl->addObject(circle);

    return app.exec();
}
