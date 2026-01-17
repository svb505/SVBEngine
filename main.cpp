#include <QApplication>
#include <QMainWindow>
#include <iostream>
#include "GUI.h"
#include "MyOpenGLWidget.h"
#include "Objects.h"
#include "HUD.h"
#include "Logger.h"
#include <GL/glut.h>

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    Logger::Init();
    LOG_INFO("[ENGINE] Engine started");

    QApplication app(argc, argv);
    QMainWindow window;
    GUI* gui = new GUI();
   

    auto* ogl = new MyOpenGLWidget(&window);
    ogl->setMode("2D");

    window.setCentralWidget(ogl);
    window.showMaximized();
    window.setWindowTitle("SVBEngine - Render: OpenGL");
    window.setWindowIcon(QIcon(":/windowLogo.png"));
    window.show();

    gui->addMenu(&window, ogl);

    return app.exec();
}
