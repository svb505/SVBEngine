#include <iostream>
#include <QTimer>
#include "ObjectsAction.h"
#include "MyOpenGLWidget.h"
#include "Objects.h"
#include <qDebug>

void Action::addObject(std::string& type, const  std::string& name, MyOpenGLWidget* ogl, const float& x,
    const  float& y, const  float& z, float colors[], std::map<std::string, float>& positions,bool& dMode) {
    qDebug() << type;
    if (type == "rectangle" || type == "flat") {
        if (ogl->mode == "2D" || type == "flat") {
            Box* obj = new Box();
            obj->position = { x, y, z };
            obj->color = { colors[0], colors[1], colors[2] };
            obj->setSize(positions["w"], positions["h"]);
            ogl->addObj(obj, name, type, x, y, z, colors[0], colors[1], colors[2]);
        }
        else {
            Cube* cube = new Cube();
            cube->position = { x, y, z };;
            cube->scale = { 1, 1, 1 };
            cube->rotation = 45.0f;
            cube->color = { colors[0], colors[1], colors[2] };
            cube->setSize(positions["w"], positions["h"], positions["w"]);
            cube->setMode(dMode);
            ogl->addObj(cube, name, "cube", cube->position.x(), cube->position.y(), cube->position.z(),
                cube->color.x(), cube->color.y(), cube->color.z());
        }
    }
    else if (type == "point") {
        Point* obj = new Point();
        obj->position = { x, y, z };
        obj->color = { colors[0], colors[1], colors[2] };
        obj->setSize(positions["size"]);
        ogl->addObj(obj, name, "point", x, y, z, colors[0], colors[1], colors[2]);
    }
    else if (type == "triangle") {
        if (ogl->mode == "2D") {
            Triangle* obj = new Triangle();
            obj->position = { x, y, z };
            obj->color = { colors[0], colors[1], colors[2] };
            obj->setSize(positions["base"], positions["h"]);
            ogl->addObj(obj, name, "triangle", x, y, z, colors[0], colors[1], colors[2]);
        }
        else {
            Pyramid* pyr = new Pyramid();
            pyr->position = { x,y,z };
            pyr->scale = { 1, 1, 1 };
            pyr->color = { colors[0], colors[1], colors[2] };
            pyr->setSize(positions["base"], positions["h"]);
            pyr->setMode(dMode);
            ogl->addObj(pyr, name, "pyramide", pyr->position.x(), pyr->position.y(), pyr->position.z(),
                pyr->color.x(), pyr->color.y(), pyr->color.z());

        }
    }
    else if (type == "circle") {
        if (ogl->mode == "2D") {
            Circle* obj = new Circle();
            obj->position = { x, y, z };
            obj->color = { colors[0], colors[1], colors[2] };
            obj->setRadius(positions["radius"]);
            ogl->addObj(obj, name, "circle", x, y, z, colors[0], colors[1], colors[2]);
        }
        else {
            Sphere* ball = new Sphere();
            ball->position = { x, y, z };
            ball->scale = { 1, 1, 1 };
            ball->color = { colors[0], colors[1], colors[2] };
            ball->setMode(dMode);
            ball->setSize(positions["radius"], 32, 32);

            ogl->addObj(ball, name, "sphere", ball->position.x(), ball->position.y(), ball->position.z(),
                ball->color.x(), ball->color.y(), ball->color.z());
        }
    }
    else if (type == "star") {
        Star* obj = new Star();
        obj->position = { x, y, z };
        obj->color = { colors[0], colors[1], colors[2] };
        obj->setSize(positions["points"], positions["outer"], positions["inner"]);
        ogl->addObj(obj, name, "star", x, y, z, colors[0], colors[1], colors[2]);
    }
    else if (type == "polygon") {
        if (ogl->mode == "2D") {
            Polygon* obj = new Polygon();
            obj->position = { x, y, z };
            obj->color = { colors[0], colors[1], colors[2] };
            obj->setSize(positions["count"], positions["radius"]);
            ogl->addObj(obj, name, "polygon", x, y, z, colors[0], colors[1], colors[2]);
        }
        else {
            Prism* prism = new Prism();
            prism->position = { x,y,z };
            prism->scale = { 1, 1, 1 };
            prism->color = { colors[0], colors[1], colors[2] };
            prism->setMode(dMode);
            prism->setSize(positions["count"], positions["radius"], positions["h"]);

            ogl->addObj(prism, name, "prism", prism->position.x(), prism->position.y(), prism->position.z(),
                prism->color.x(), prism->color.y(), prism->color.z());
        }
    }
    else if (type == "line") {
        Line* obj = new Line();
        obj->position = { x, y, z };
        obj->color = { colors[0], colors[1], colors[2] };
        obj->setSize(positions["wL"], positions["x0"], positions["y0"], positions["lineW"]);
        ogl->addObj(obj, name, "line", x, y, z, colors[0], colors[1], colors[2]);
    }
    else if (type == "cone") {
        Cone* obj = new Cone();
        obj->position = { x, y, z };
        obj->color = { colors[0], colors[1], colors[2] };
        obj->setSize(positions["r"], positions["h"]);
        obj->setMode(dMode);
        ogl->addObj(obj, name, "cone", x, y, z, colors[0], colors[1], colors[2]);
    }
    else if (type == "cylinder") {
        Cylinder* obj = new Cylinder();
        obj->position = { x, y, z };
        obj->color = { colors[0], colors[1], colors[2] };
        obj->setSize(positions["rT"], positions["rB"], positions["h"]);
        obj->setMode(dMode);
        ogl->addObj(obj, name, "cylinder", x, y, z, colors[0], colors[1], colors[2]);
    }
}
void Action::movingParcer(MyOpenGLWidget* ogl, const std::string& name, const int& repeatTime, const QStringList& items,
    const std::map<std::string, std::string>& scenarios, const int& speed)
{
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

        startMoveObj(
            ogl,
            name,
            speed,
            (*queue)[index].vect,
            (*queue)[index].num,
            repeatTime,
            [=]() { (*runNextPtr)(index + 1); }
        );
        };

    (*runNextPtr)(0);
}
void Action::startMoveObj(MyOpenGLWidget* ogl, const std::string& name, const int& speed, const std::string& vect, const int time, const int to, std::function<void()> onFinished)
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

        if (vect == "LEFT") {
            x -= currentStep;
        }
        else if (vect == "RIGHT") {
            x += currentStep;
        }
        else if (vect == "TOP") {
            y += currentStep;
        }
        else if (vect == "BOTTOM") {
            y -= currentStep;
        }
        else if (vect == "DLEFTTOP") {
            x -= currentStep;
            y += currentStep;
        }
        else if (vect == "DLEFTBOTTOM") {
            x -= currentStep;
            y -= currentStep;
        }
        else if (vect == "DRIGHTTOP") {
            x += currentStep;
            y += currentStep;
        }
        else if (vect == "DRIGHTBOTTOM") {
            x += currentStep;
            y -= currentStep;
        }

        moved += currentStep;
        ogl->moveObj(name, x, y, z);

        if (moved >= to) {
            moved = 0;
            repeatCount++;
        }
        });

    timer->start();
}