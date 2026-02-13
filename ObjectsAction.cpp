#include <iostream>
#include <QTimer>
#include "ObjectsAction.h"
#include "OpenGLW.h"
#include "Objects.h"
#include <qDebug>

void Action::addObject(std::string& type, const  std::string& name, OpenGLW* ogl, const float& x,
    const  float& y, const  float& z, float colors[], std::map<std::string, float>& positions,bool& dMode,
    std::string parent) {

    LOG_INFO(std::format("[SCENE] Object '{}' added, settings: wireframeMode:{}, type: {},x: {}, y: {}, z: {}, R: {}, G: {}, B: {}",
        name,dMode,type,x,y,z,colors[0],colors[1],colors[2]));

    Object* obj = nullptr;
    std::string realType = type;

    float pos[3] = {x, y, z};
    float col[3] = {colors[0], colors[1], colors[2]};

    float turnX = positions["turnX"];
    float turnY = positions["turnY"];
    float turnZ = positions["turnZ"];


    if (type == "rectangle" || type == "flat" || type == "platform") {

        if (ogl->mode == "2D" || type == "flat") {
            obj = new Box();
            static_cast<Box*>(obj)->setSize(positions["w"], positions["h"]);
        }
        else if (type == "platform") {
            obj = new Platform();
            static_cast<Platform*>(obj)->setSize(positions["w"], positions["h"]);
            obj->scale = { 1,1,1 };
            obj->rotation = 45.f;
            obj->mode = dMode;
            realType = "platform";
        }
        else {
            obj = new Cube();
            static_cast<Cube*>(obj)->setSize(
                positions["w"],
                positions["h"],
                positions["w"]
            );
            obj->scale = { 1,1,1 };
            obj->rotation = 45.f;
            obj->mode = dMode;
            realType = "cube";
        }
    }
    else if (type == "point") {
        obj = new Point();
        static_cast<Point*>(obj)->setSize(positions["size"]);
    }
    else if (type == "triangle") {

        if (ogl->mode == "2D") {
            obj = new Triangle();
            static_cast<Triangle*>(obj)->setSize(
                positions["base"],
                positions["h"]
            );
        }
        else {
            obj = new Pyramid();
            static_cast<Pyramid*>(obj)->setSize(
                positions["base"],
                positions["h"]
            );
            obj->scale = { 1,1,1 };
            obj->mode = dMode;
            realType = "pyramid";
        }
    }
    else if (type == "circle") {

        if (ogl->mode == "2D") {
            obj = new Circle();
            static_cast<Circle*>(obj)->setRadius(
                positions["radius"]
            );
        }
        else {
            obj = new Sphere();
            static_cast<Sphere*>(obj)->setSize(
                positions["radius"],
                32,
                32
            );
            obj->scale = { 1,1,1 };
            obj->mode = dMode;
            realType = "sphere";
        }
    }
    else if (type == "star") {
        obj = new Star();
        static_cast<Star*>(obj)->setSize(
            positions["points"],
            positions["outer"],
            positions["inner"]
        );
    }
    else if (type == "polygon") {
        if (ogl->mode == "2D") {
            obj = new PolygonFigure();
            static_cast<PolygonFigure*>(obj)->setSize(
                positions["count"],
                positions["radius"]
            );
        }
        else {
            obj = new Prism();
            static_cast<Prism*>(obj)->setSize(
                positions["count"],
                positions["radius"],
                positions["h"]
            );
            obj->scale = { 1,1,1 };
            obj->mode = dMode;
            realType = "prism";
        }
    }
    else if (type == "line") {
        obj = new Line();
        static_cast<Line*>(obj)->setSize(
            positions["wL"],
            positions["x0"],
            positions["y0"],
            positions["lineW"]
        );
    }
    else if (type == "cone") {
        obj = new Cone();
        static_cast<Cone*>(obj)->setSize(
            positions["r"],
            positions["h"]
        );
        obj->mode = dMode;
    }
    else if (type == "cylinder") {
        obj = new Cylinder();
        static_cast<Cylinder*>(obj)->setSize(
            positions["rT"],
            positions["rB"],
            positions["h"]
        );
        obj->mode = dMode;
    }

    if (obj) {
        obj->position = { x, y, z };
        obj->color = { colors[0], colors[1], colors[2] };
        obj->turnX = turnX;
        obj->turnY = turnY;
        obj->turnZ = turnZ;

        ogl->addObj(obj, name, realType,pos[0], pos[1], pos[2],col[0], col[1], col[2],parent);
    }
}
void Action::movingParcer(OpenGLW* ogl, const std::string& name, const int& repeatTime, const QStringList& items,
    const std::map<std::string, std::string>& scenarios, const int& speed){
    auto queue = std::make_shared<QVector<Scenario>>();

    for (const QString& i : items) {
        QString textOnly = i.section(" on ", 0, 0);
        std::string vect;

        for (const auto& [key, value] : scenarios) {
            QString valueOnly = QString::fromStdString(value).section(" on", 0, 0);
            if (textOnly == valueOnly.toStdString()) {
                vect = key;
                break;
            }
        }

        int num = i.section(" on ", 1, 1).toInt();
        queue->push_back({ vect, num });
    }
    auto runNextPtr = std::make_shared<std::function<void(int)>>();

    *runNextPtr = [=](int index) {
        if (index >= queue->size()) return;

        startMoveObj(ogl,name,speed,(*queue)[index].vect,(*queue)[index].num,repeatTime,[=]() 
            { (*runNextPtr)(index + 1); });
        };

    (*runNextPtr)(0);
}
void Action::startMoveObj(OpenGLW* ogl, const std::string& name, const int& speed, const std::string& vect, const int time, const int to, std::function<void()> onFinished)
{
    int x = ogl->getX(name);
    int y = ogl->getY(name);
    int z = ogl->getZ(name);

    int step = speed;
    int repeatCount = 0;
    int moved = 0;

    QTimer* timer = new QTimer(ogl);
    timer->setInterval(1000 / speed);

    QObject::connect(timer, &QTimer::timeout, [=]() mutable {
        if (repeatCount >= time || moved >= to) {
            timer->stop();
            timer->deleteLater();
            if (onFinished) onFinished();
            return;
        }

        int currentStep = std::min(step, to - moved);

        if (vect == "LEFT") x -= currentStep;
        else if (vect == "RIGHT") x += currentStep;
        else if (vect == "TOP") y += currentStep;
        else if (vect == "BOTTOM") y -= currentStep;
        else if (vect == "DLEFTTOP") { x -= currentStep;y += currentStep; }
        else if (vect == "DLEFTBOTTOM") { x -= currentStep; y -= currentStep; }
        else if (vect == "DRIGHTTOP") { x += currentStep;y += currentStep; }
        else if (vect == "DRIGHTBOTTOM") { x += currentStep; y -= currentStep;}

        moved += currentStep;
        ogl->moveObj(name, x, y, z);

        if (moved >= to) {
            moved = 0;
            repeatCount++;
        }

        });

    timer->start();
}