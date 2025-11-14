#include <QApplication>
#include <QMainWindow>
#include "MyOpenGLWidget.h"
#include "Objects.h"
#include <iostream>
#include <QMenu>
#include <QMenuBar>

class GUI {
public:
    void addMenu(QMainWindow* w) {
        QMenuBar* menubar = w->menuBar();
        QMenu* importMenu = menubar->addMenu("Import");
        QMenu* exportMenu = menubar->addMenu("Export");
        QMenu* objectsMenu = menubar->addMenu("Objects");
        QAction* scenariosMenu = menubar->addAction("Scenarios");
        QAction* physicsMenu = menubar->addAction("Physics simulation");

        QAction* cubeAction = objectsMenu->addAction("Add cube");
        QAction* rectAction = objectsMenu->addAction("Add rectangle");
        QAction* circleAction = objectsMenu->addAction("Add circle");
        QAction* trianglAction = objectsMenu->addAction("Add triangle");
    }
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QMainWindow window;
    auto* ogl = new MyOpenGLWidget();
    ogl->setMode("2D");
    window.setCentralWidget(ogl);
    window.showMaximized();
    window.setWindowTitle("SVBEngine - OpenGL");
    window.show();

    GUI gui;
    gui.addMenu(&window);

    Box* box = new Box();
    box->position = { 30, 0, 0};
    box->color = { 255, 0, 0 };
    box->setSize(20.0f, 20.0f);
    ogl->addObj(box, "box1", 30, 0, 0, 225.0f,0.0f,0.0f);

    Circle* circle = new Circle();
    circle->position = { 100, 0, 0 };
    circle->color = { 0, 255, 0 };
    circle->setRadius(6.0f);
    ogl->addObj(circle, "circle1", 190, 0, 0, 0.0f, 255.0f, 0.0f);

    return app.exec();
}
