#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>
#include <GUI.h>
#include <MyOpenGLWidget.h>
#include <QTimer>

/*QTimer* timer = new QTimer(ogl);

        QObject::connect(timer, &QTimer::timeout, ogl, [=]() mutable {

            x += 10;
            ogl->moveObj(obj, "triangle1", x, 90, 0, 225, 0, 170);

            if (x >= 100) {
                timer->stop();
                timer->deleteLater();
            }
            });

        timer->start(50);*/

void GUI::addMenu(QMainWindow* w,MyOpenGLWidget* ogl) {
    QMenuBar* menubar = w->menuBar();
    QAction* importAction = menubar->addAction("Import");
    QAction* exportAction = menubar->addAction("Export");
    QMenu* modeMenu = menubar->addMenu("Mode");
    QMenu* objectsMenu = menubar->addMenu("Objects");
    QAction* sceneAction = menubar->addAction("Scene");
    QAction* scenariosAction = menubar->addAction("Scenarios");
    QAction* physicsAction = menubar->addAction("Physics simulation");

    QAction* rectAction = objectsMenu->addAction("Add rectangle");
    QAction* circleAction = objectsMenu->addAction("Add circle");
    QAction* trianglAction = objectsMenu->addAction("Add triangle");
    QAction* starAction = objectsMenu->addAction("Add start");
    QAction* polyAction = objectsMenu->addAction("Add polygon");
    QAction* lineAction = objectsMenu->addAction("Add line");

    QAction* twoD= modeMenu->addAction("Set 2D mode");
    QAction* threeD = modeMenu->addAction("Set 3D mode");

    QObject::connect(physicsAction, &QAction::triggered, [&]() {
        openPhysicsWindow();
    });
    QObject::connect(scenariosAction, &QAction::triggered, [&]() {
        openScenariosWindow();
        });
    QObject::connect(sceneAction, &QAction::triggered, [&]() {
        openSceneWindow();
        });
    QObject::connect(rectAction, &QAction::triggered, [&]() {
        addObjWindow("rectangle",ogl);
        });
    QObject::connect(circleAction, &QAction::triggered, [&]() {
        addObjWindow("circle", ogl);
        });
    QObject::connect(trianglAction, &QAction::triggered, [&]() {
        addObjWindow("triangle",ogl);
        });
    QObject::connect(starAction, &QAction::triggered, [&]() {
        addObjWindow("star", ogl);
        });
    QObject::connect(polyAction, &QAction::triggered, [&]() {
        addObjWindow("polygon", ogl);
        });
    QObject::connect(lineAction, &QAction::triggered, [&]() {
        addObjWindow("line", ogl);
        });
    QObject::connect(twoD, &QAction::triggered, [this,ogl]() {
        changeMode("2D",ogl);
        });
    QObject::connect(threeD, &QAction::triggered, [this,ogl]() {
        changeMode("3D",ogl);
        });
};
void GUI::openPhysicsWindow() {
    //
};
void GUI::openSceneWindow() {
    //
};
void GUI::openScenariosWindow() {
    //
};
void GUI::addObjWindow(const std::string& type, MyOpenGLWidget* ogl) {
    //temp data
    if (type == "rectangle") {
        Box* obj = new Box();
        obj->position = { 30, 40, 0 };
        obj->color = { 255, 0, 170 };
        obj->setSize(40.0f, 20.0f);

        ogl->addObj(obj, "rectangle1", 30, 40, 0, 225.0f, 0.0f, 170.0f);
    }
    else if (type == "triangle") {
        Triangle* obj = new Triangle();
        obj->position = { 30, 90, 0 };
        obj->color = { 255, 0, 170 };
        obj->setSize(50.0f, 50.0f);
        ogl->addObj(obj, "triangle1", 30, 90, 0, 225, 0, 170);
    }
    else if (type == "circle") {
        Circle* circle = new Circle();
        circle->position = { 100, 0, 0 };
        circle->color = { 0, 255, 0 };
        circle->setRadius(6.0f);
        ogl->addObj(circle, "circle1", 190, 0, 0, 0.0f, 255.0f, 0.0f);
    }
    else if (type == "star") {
        Star* obj = new Star();
        obj->position = { -140, 10, 0 };
        obj->color = { 255, 130, 170 };
        obj->setSize(6, 40.0f,40.0f);
        ogl->addObj(obj, "star1", -140, 10, 0, 225, 130, 170);
    }
    else if (type == "polygon") {
        Polygon* obj = new Polygon();
        obj->position = { 110, 10, 0 };
        obj->color = { 255, 190, 170 };
        obj->setSize(6, 30.0f);
        ogl->addObj(obj, "polygon1", 110, 10, 0, 225, 130, 170);
    }
    else if (type == "line") {
        Line* obj = new Line();
        obj->position = { 0, 0, 0 };
        obj->color = { 255, 190, 180 };
        obj->setSize(60, 0, 0, 4.0f);
        ogl->addObj(obj, "line1", 0, 0, 0, 225, 190, 180);
    }
}
void GUI::changeMode(const std::string& mode,MyOpenGLWidget* ogl) {
    ogl->setMode(mode);
};