#include <QApplication>
#include <QMainWindow>
#include <iostream>
#include <GUI.h>
#include "MyOpenGLWidget.h"
#include "Objects.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QMainWindow window;
    GUI gui;

    auto* ogl = new MyOpenGLWidget(&window);

    window.setCentralWidget(ogl);
    window.showMaximized();
    window.setWindowTitle("SVBEngine - Render: OpenGL");
    window.setWindowIcon(QIcon(":/logo.png"));
    window.show();

    ogl->setMode("3D");


    gui.addMenu(&window,ogl);

    return app.exec();
}
