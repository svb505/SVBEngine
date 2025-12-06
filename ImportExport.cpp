#include <iostream>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QDir>
#include <QListWidget>
#include "ImportExport.h"
#include "Objects.h"

std::map<std::string, Data> ImpExp::importScene(const QString& fileName, MyOpenGLWidget* ogl)
{
    std::map<std::string, Data> result;

    QFile file("scenes/" + fileName + ".json");
    if (!file.open(QIODevice::ReadOnly))
        return result;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc["mode"].isString())
        ogl->setMode(doc["mode"].toString().toStdString());
    else
        ogl->setMode("2D");
    QJsonArray arr = doc["objects"].toArray();

    for (const auto& v : arr)
    {
        QJsonObject o = v.toObject();

        std::string name = o["name"].toString().toStdString();
        std::string type = o["type"].toString().toStdString();

        QJsonObject pos = o["position"].toObject();
        QJsonObject col = o["color"].toObject();
        QJsonObject size = o["size"].toObject();

        Data d;
        d.x = pos["x"].toDouble();
        d.y = pos["y"].toDouble();
        d.z = pos["z"].toDouble();

        d.r = col["r"].toDouble();
        d.g = col["g"].toDouble();
        d.b = col["b"].toDouble();

        Object* obj = nullptr;

        if (type == "rectangle") { Box* b = new Box(); b->setSize(size["width"].toDouble(), size["height"].toDouble()); obj = b; }
        else if (type == "circle") { Circle* c = new Circle(); c->setRadius(size["radius"].toDouble()); obj = c; }
        else if (type == "triangle") { Triangle* t = new Triangle(); t->setSize(size["base"].toDouble(), size["height"].toDouble()); obj = t; }
        else if (type == "polygon") { Polygon* p = new Polygon(); p->setSize(size["sides"].toInt(), size["radius"].toDouble()); obj = p; }
        else if (type == "star") { Star* s = new Star(); s->setSize(size["points"].toInt(), size["outer"].toDouble(), size["inner"].toDouble()); obj = s; }
        else if (type == "line") { Line* ln = new Line(); ln->setSize(size["width"].toInt(), size["x0"].toInt(), size["y0"].toInt(), size["lineW"].toDouble()); obj = ln; }
        else if (type == "cube") { Cube* c = new Cube(); c->setSize(size["width"].toDouble(), size["height"].toDouble(), size["depth"].toDouble()); c->mode = size["mode"].toDouble(); obj = c; }
        else if (type == "sphere") { Sphere* s = new Sphere(); s->setSize(size["radius"].toDouble(), size["slices"].toInt(), size["stacks"].toInt()); s->mode = size["mode"].toDouble(); obj = s; }
        else if (type == "pyramide") { Pyramid* p = new Pyramid(); p->setSize(size["base"].toDouble(), size["height"].toDouble()); p->mode = size["mode"].toDouble(); obj = p; }
        else if (type == "prism") { Prism* p = new Prism(); p->setSize(size["sides"].toInt(), size["radius"].toDouble(), size["height"].toDouble()); p->mode = size["mode"].toDouble(); obj = p; }
        else if (type == "cone") { Cone* p = new Cone(); p->setSize(size["radius"].toDouble(), size["h"].toDouble()); p->mode = size["mode"].toDouble(); obj = p; }
        else if (type == "cylinder") { Cylinder* p = new Cylinder(); p->setSize(size["rTop"].toInt(), size["rBottom"].toDouble(), size["h"].toDouble()); p->mode = size["mode"].toDouble(); obj = p; }
        else if (type == "point") { Point* p = new Point(); p->setSize(size["size"].toInt()); obj = p;}
        obj->position = { d.x, d.y, d.z };
        obj->color = { d.r, d.g, d.b };
        
        ogl->addObj(obj, name, type, d.x, d.y, d.z, d.r, d.g, d.b);

        d.obj = obj;
        result[name] = d;
    }

    return result;
}
void ImpExp::exportScene(const QString& fileName, const std::map<std::string, Data>& objects, MyOpenGLWidget* ogl)
{
    QJsonArray objectArray;

    for (const auto& [name, data] : objects)
    {
        Object* obj = data.obj;
        if (!obj) continue;

        QJsonObject o;
        o["name"] = QString::fromStdString(name);
        QString type = QString::fromStdString(ogl->getType(name));
        o["type"] = type;

        QJsonObject pos;
        pos["x"] = data.x;
        pos["y"] = data.y;
        pos["z"] = data.z;
        o["position"] = pos;

        QJsonObject col;
        col["r"] = data.r;
        col["g"] = data.g;
        col["b"] = data.b;
        o["color"] = col;

        QJsonObject size;

        if (type == "rectangle") {
            Box* b = dynamic_cast<Box*>(obj);
            size["width"] = b->getW();
            size["height"] = b->getH();
        }
        else if (type == "point") {
            Point* b = dynamic_cast<Point*>(obj);
            size["size"] = b->getSize();
        }
        else if (type == "circle") {
            Circle* c = dynamic_cast<Circle*>(obj);
            size["radius"] = c->getRadius();
        }
        else if (type == "triangle") {
            Triangle* t = dynamic_cast<Triangle*>(obj);
            size["base"] = t->getBase();
            size["height"] = t->getH();
        }
        else if (type == "polygon") {
            Polygon* p = dynamic_cast<Polygon*>(obj);
            size["sides"] = p->getSides();
            size["radius"] = p->getRadius();
        }
        else if (type == "star") {
            Star* s = dynamic_cast<Star*>(obj);
            size["points"] = s->getPoints();
            size["outer"] = s->getOuter();
            size["inner"] = s->getInner();

        }
        else if (type == "line") {
            Line* ln = dynamic_cast<Line*>(obj);
            size["width"] = ln->getWidth();
            size["x0"] = ln->getX0();
            size["y0"] = ln->getY0();
            size["lineW"] = ln->getLineW();
        }
        else if (type == "cube") {
            Cube* c = dynamic_cast<Cube*>(obj);
            size["width"] = c->width;
            size["height"] = c->height;
            size["depth"] = c->depth;
            size["mode"] = c->getMode();
        }
        else if (type == "sphere") {
            Sphere* s = dynamic_cast<Sphere*>(obj);
            size["radius"] = s->getRadius();
            size["slices"] = s->getSlices();
            size["stacks"] = s->getStacks();
            size["mode"] = s->getMode();
        }
        else if (type == "pyramid") {
            Pyramid* p = dynamic_cast<Pyramid*>(obj);
            size["base"] = p->base;
            size["height"] = p->height;
            size["mode"] = p->getMode();
        }
        else if (type == "prism") {
            Prism* p = dynamic_cast<Prism*>(obj);
            size["sides"] = p->getSides();
            size["radius"] = p->getRadius();
            size["height"] = p->getHeight();
            size["mode"] = p->getMode();
        }
        else if (type == "cylinder") {
            Cylinder* p = dynamic_cast<Cylinder*>(obj);
            size["rTop"] = p->getRT();
            size["rBottom"] = p->getRB();
            size["mode"] = p->getMode();
            size["h"] = p->getH();
        }
        else if (type == "cone") {
            Cone* p = dynamic_cast<Cone*>(obj);
            size["radius"] = p->getR();
            size["mode"] = p->getMode();
            size["h"] = p->getH();
        }

        o["size"] = size;
        objectArray.append(o);
    }

    QJsonObject root;
    root["objects"] = objectArray;
    root["mode"] = QString::fromStdString(ogl->mode);

    QDir().mkpath("scenes");

    QFile file("scenes/" + fileName + ".json");
    if (file.open(QIODevice::WriteOnly))
        file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
}
void ImpExp::exportSceneWithDialog(const std::map<std::string, Data>& objects, MyOpenGLWidget* ogl)
{
    QString fileName = QFileDialog::getSaveFileName(
        nullptr,
        "Save scene",
        "scenes/scene.json",
        "Scene Files (*.json)"
    );

    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".json"))
        fileName += ".json";

    QFileInfo fi(fileName);
    QDir().mkpath(fi.dir().path());

    exportScene(fi.baseName(), objects, ogl);
}
std::map<std::string, Data> ImpExp::importSceneWithDialog(MyOpenGLWidget* ogl)
{
    QString fileName = QFileDialog::getOpenFileName(
        nullptr,
        "Open scene",
        "scenes/",
        "Scene Files (*.json)"
    );

    if (fileName.isEmpty())
        return {};

    QFileInfo fi(fileName);
    return importScene(fi.baseName(), ogl);
}
void ImpExp::exportScenarios(QWidget* child, QListWidget* list1) {
    QString fileName = QFileDialog::getSaveFileName(
        child,
        "Export scenarios",
        "scenarios/",
        "JSON (*.json)"
    );
    if (fileName.isEmpty()) return;

    QJsonArray arr;
    for (int i = 0; i < list1->count(); i++) {
        arr.append(list1->item(i)->text());
    }

    QJsonObject root;
    root["scenarios"] = arr;

    QFile f(fileName);
    if (f.open(QIODevice::WriteOnly)) {
        f.write(QJsonDocument(root).toJson());
        f.close();
    }
}
void ImpExp::importScenarios(QWidget* child, QListWidget* list1) {
    QString fileName = QFileDialog::getOpenFileName(
        child,
        "Import scenarios",
        "scenarios/",
        "JSON (*.json)"
    );
    if (fileName.isEmpty()) return;

    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    f.close();
    if (!doc.isObject()) return;

    QJsonArray arr = doc.object()["scenarios"].toArray();

    list1->clear();
    for (auto v : arr) {
        if (v.isString())
            list1->addItem(v.toString());
    }
}