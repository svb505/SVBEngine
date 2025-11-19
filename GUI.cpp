#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>
#include <QTimer>
#include <QLineEdit>
#include <QColorDialog>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QListWidget>
#include <QPointer>
#include <GUI.h>
#include <MyOpenGLWidget.h>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QDir>

std::map<std::string, Data> importScene(const QString& fileName, MyOpenGLWidget* ogl)
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

        // создаём объект по типу (как у тебя было)
        if (type == "rectangle") { Box* b = new Box(); b->setSize(size["width"].toDouble(), size["height"].toDouble()); obj = b; }
        else if (type == "circle") { Circle* c = new Circle(); c->setRadius(size["radius"].toDouble()); obj = c; }
        else if (type == "triangle") { Triangle* t = new Triangle(); t->setSize(size["base"].toDouble(), size["height"].toDouble()); obj = t; }
        else if (type == "polygon") { Polygon* p = new Polygon(); p->setSize(size["sides"].toInt(), size["radius"].toDouble()); obj = p; }
        else if (type == "star") { Star* s = new Star(); s->setSize(size["points"].toInt(), size["outer"].toDouble(), size["inner"].toDouble()); obj = s; }
        else if (type == "line") { Line* ln = new Line(); ln->setSize(size["width"].toInt(), size["x0"].toInt(), size["y0"].toInt(), size["lineW"].toDouble()); obj = ln; }
        else if (type == "cube") { Cube* c = new Cube(); c->setSize(size["width"].toDouble(), size["height"].toDouble(), size["depth"].toDouble()); obj = c; }
        else if (type == "sphere") { Sphere* s = new Sphere(); s->setSize(size["radius"].toDouble(), size["slices"].toInt(), size["stacks"].toInt()); obj = s; }
        else if (type == "pyramid") { Pyramid* p = new Pyramid(); p->setSize(size["base"].toDouble(), size["height"].toDouble()); obj = p; }
        else if (type == "prism") { Prism* p = new Prism(); p->setSize(size["sides"].toInt(), size["radius"].toDouble(), size["height"].toDouble()); obj = p; }

        obj->position = { d.x, d.y, d.z };
        obj->color = { d.r, d.g, d.b };

        ogl->addObj(obj, name, type, d.x, d.y, d.z, d.r, d.g, d.b);

        d.obj = obj;
        result[name] = d;
    }

    return result;
}
void exportScene(const QString& fileName, const std::map<std::string, Data>& objects, MyOpenGLWidget* ogl)
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
        }
        else if (type == "sphere") {
            Sphere* s = dynamic_cast<Sphere*>(obj);
            size["radius"] = s->getRadius();
            size["slices"] = s->getSlices();
            size["stacks"] = s->getStacks();
        }
        else if (type == "pyramid") {
            Pyramid* p = dynamic_cast<Pyramid*>(obj);
            size["base"] = p->base;
            size["height"] = p->height;
        }
        else if (type == "prism") {
            Prism* p = dynamic_cast<Prism*>(obj);
            size["sides"] = p->getSides();
            size["radius"] = p->getRadius();
            size["height"] = p->getHeight();
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

void exportSceneWithDialog(const std::map<std::string, Data>& objects, MyOpenGLWidget* ogl)
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
std::map<std::string, Data> importSceneWithDialog(MyOpenGLWidget* ogl)
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

QLabel* GUI::makeLabel(QWidget* parent, const QString& text, const int& x, const int& y) {
    QLabel* lbl = new QLabel(parent);
    lbl->setText(text);
    lbl->move(x,y);
    lbl->show();
    return lbl;
}
QLineEdit* GUI::makeLineEdit(QWidget* parent, const QString& text, const QString& setTxt, const int& x, const int& y) {
    QLineEdit* entry = new QLineEdit(parent);
    entry->setPlaceholderText(text);
    entry->setText(setTxt);
    entry->move(x, y);
    entry->show();
    return entry;
}
void GUI::addMenu(QMainWindow* w,MyOpenGLWidget* ogl) {
    QMenuBar* menubar = w->menuBar();
    QAction* importAction = menubar->addAction("Import");
    QAction* exportAction = menubar->addAction("Export");
    QMenu* modeMenu = menubar->addMenu("Mode");
    QMenu* objectsMenu = menubar->addMenu("Objects");
    QMenu* addMenu = objectsMenu->addMenu("Add");
    QAction* removeAction = objectsMenu->addAction("Remove");
    QAction* sceneAction = menubar->addAction("Scene");
    QAction* scenariosAction = menubar->addAction("Scenarios");    

    QAction* rectAction = addMenu->addAction("Add rectangle");
    QAction* circleAction = addMenu->addAction("Add circle");
    QAction* trianglAction = addMenu->addAction("Add triangle");
    QAction* starAction = addMenu->addAction("Add star");
    QAction* polyAction = addMenu->addAction("Add polygon");
    QAction* lineAction = addMenu->addAction("Add line");

    QAction* twoD= modeMenu->addAction("Set 2D mode");
    QAction* threeD = modeMenu->addAction("Set 3D mode");
    QObject::connect(removeAction, &QAction::triggered, [&]() {
        openRemoveWindow(ogl);
        });
    QObject::connect(scenariosAction, &QAction::triggered, [&]() {
        openScenariosWindow(ogl);
        });
    QObject::connect(sceneAction, &QAction::triggered, [&]() {
        openSceneWindow(ogl);
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
    QObject::connect(importAction, &QAction::triggered, [this, ogl]() {
        importSceneWithDialog(ogl);
        });
    QObject::connect(exportAction, &QAction::triggered, [this, ogl]() {
        exportSceneWithDialog(ogl->getObjects(),ogl);
        });
};
void GUI::openSceneWindow(MyOpenGLWidget* ogl) {
    auto params = ogl->getProjectionParams();
    QWidget* child = new QWidget();
    child->resize(200,150);
    child->setWindowTitle("Scene info");
    child->show();

    std::string lR = std::format("X from {:.1f}; X to {:.1f}", params.left, params.right);
    std::string bT = std::format("Y from {:.1f}; Y to {:.1f}", params.bottom, params.top);
    std::string znZF = std::format("Z from {:.1f}; Z to {:.1f}", params.zNear, params.zFar);

    QLabel* infoScene = makeLabel(child,"Scene info",10,10);

    QLabel* infoX = makeLabel(child, QString::fromStdString(lR), 10, 40);
    QLabel* infoY = makeLabel(child, QString::fromStdString(bT), 10, 70);
    QLabel* infoZ = makeLabel(child, QString::fromStdString(znZF), 10, 100);
};
void GUI::openScenariosWindow(MyOpenGLWidget* ogl) {
    std::map<std::string, std::string> scenarios;
    scenarios["LEFT"] = "Moving to left on";
    scenarios["RIGHT"] = "Moving to right on";
    scenarios["TOP"] = "Moving to top on";
    scenarios["BOTTOM"] = "Moving to bottom on";
    scenarios["DLEFTTOP"] = "Moving to diagonal(left -> top) on";
    scenarios["DLEFTBOTTOM"] = "Moving to diagonal(left -> bottom) on";
    scenarios["DRIGHTTOP"] = "Moving to diagonal(right -> top) on";
    scenarios["DRIGHTBOTTOM"] = "Moving to diagonal(right -> bottom) on";


    QWidget* child = new QWidget();
    child->resize(550, 400);
    child->setWindowTitle("Scenarios");
    child->show();

    QLabel* lbl1 = makeLabel(child, "Select object", 10, 10);

    QComboBox* combo1 = new QComboBox(child);
    combo1->move(10, 30);
    for (auto name : ogl->getObjects()) {
        combo1->addItem(QString::fromStdString(name.first));
    }
    combo1->show();
    QLabel* pix = makeLabel(child, "Move on ... pixels", 10, 55);
    QLineEdit* pixels = makeLineEdit(child, "On", "10", 10, 70);

    QLabel* lbl2 = makeLabel(child, "Select scenario", 100, 10);

    QComboBox* combo2 = new QComboBox(child);
    combo2->move(100, 30);
    for (auto name : scenarios) {
        combo2->addItem(QString::fromStdString(name.second));
    }
    combo2->show();

    QLabel* lbl3 = makeLabel(child, "Added scenarios", 340, 10);
    QListWidget* list1 = new QListWidget(child);
    list1->move(350, 30);
    list1->resize(190, 100);
    list1->show();

    QPushButton* add = new QPushButton(child);
    add->setText("Add scenario");
    add->move(10,95);
    add->show();

    QObject::connect(add, &QPushButton::clicked, [combo2, list1, pixels]() {
        QString item = combo2->currentText();
        bool ok;
        int pix = pixels->text().toInt(&ok);
        if (!ok) pix = 0;

        QString str = QString("%1 %2").arg(item).arg(pix);
        list1->addItem(str);
        });


    QLabel* lbl4 = makeLabel(child, "How many times to repeat", 10, 120);
    QComboBox* combo3 = new QComboBox(child);
    combo3->move(10, 135);
    combo3->addItem("1");
    combo3->addItem("2");
    combo3->addItem("5");
    combo3->addItem("10");
    combo3->show();

    QLabel* lbl5 = makeLabel(child, "Enter speed", 160, 120);
    QLineEdit* speed = makeLineEdit(child, "Enter speed", "5", 160, 135);

    QPushButton* start = new QPushButton(child);
    start->setText("Start");
    start->move(10, 160);
    start->show();

    QObject::connect(start, &QPushButton::clicked, [this,ogl,combo1,combo3,list1,scenarios,pixels,speed]() {
        QStringList texts;
        for (int i = 0; i < list1->count(); i++) {
            texts << list1->item(i)->text();
        }
        movingParcer(ogl,combo1->currentText().toStdString(), combo3->currentText().toInt(), texts,
            scenarios,speed->text().toInt());
    });

};
void GUI::openRemoveWindow(MyOpenGLWidget* ogl) {
    QWidget* child = new QWidget();
    child->setWindowTitle("Removing object");
    child->resize(250, 150);
    child->show();

    QLabel* lbl = makeLabel(child, "Avaibles objects",10,10);

    QComboBox* combo = new QComboBox(child);
    combo->move(10, 30);
    for (auto name : ogl->getObjects()) {
        combo->addItem(QString::fromStdString(name.first));
    }
    combo->show();
    
    
    QPushButton* btn = new QPushButton(child);
    btn->setText("Remove object");
    btn->move(10, 50);
    btn->show();

    QObject::connect(btn, &QPushButton::clicked, [combo,ogl]() {
        if (combo->currentIndex() >= 0) {
            ogl->removeObj(combo->currentText().toStdString());
        }
        else {
            QMessageBox::warning(nullptr,"Empty choice","Choice the object from combobox!");
        }
    });



}
void GUI::addObject(std::string& type, const  std::string& name,MyOpenGLWidget* ogl, const float& x,
    const  float& y, const  float& z,float colors[],std::map<std::string,float>& positions) {
    if (type == "rectangle") {
        if (ogl->mode == "2D") {
            Box* obj = new Box();
            obj->position = { x, y, z };
            obj->color = { colors[0], colors[1], colors[2] };
            obj->setSize(positions["w"], positions["h"]);
            ogl->addObj(obj, name, "rectangle",x, y, z, colors[0], colors[1], colors[2]);
        }
        else {
            Cube* cube = new Cube();
            cube->position = { x, y, z };;
            cube->scale = { 1, 1, 1 };       
            cube->rotation = 45.0f;
            cube->color = { colors[0], colors[1], colors[2] };
            cube->setSize(positions["w"], positions["h"], positions["w"]);
            ogl->addObj(cube, name, "cube",cube->position.x(), cube->position.y(), cube->position.z(),
                cube->color.x(), cube->color.y(), cube->color.z());
        }
    }
    else if (type == "triangle") {
        if (ogl->mode == "2D") {
            Triangle* obj = new Triangle();
            obj->position = { x, y, z };
            obj->color = { colors[0], colors[1], colors[2] };
            obj->setSize(positions["base"], positions["h"]);
            ogl->addObj(obj, name, "triangle",x, y, z, colors[0], colors[1], colors[2]);
        }
        else {
            Pyramid* pyr = new Pyramid();
            pyr->position = { x,y,z };
            pyr->scale = { 1, 1, 1 };
            pyr->color = { colors[0], colors[1], colors[2] };
            pyr->setSize(positions["base"], positions["h"]);  

            ogl->addObj(pyr, name, "pyramide",pyr->position.x(), pyr->position.y(), pyr->position.z(),
                pyr->color.x(), pyr->color.y(), pyr->color.z());

        }
    }
    else if (type == "circle") {
        if (ogl->mode == "2D") {
            Circle* obj = new Circle();
            obj->position = { x, y, z };
            obj->color = { colors[0], colors[1], colors[2] };
            obj->setRadius(positions["radius"]);
            ogl->addObj(obj, name, "circle",x, y, z, colors[0], colors[1], colors[2]);
        }
        else {
            Sphere* ball = new Sphere();
            ball->position = { x, y, z };
            ball->scale = { 1, 1, 1 };
            ball->color = { colors[0], colors[1], colors[2] };
            ball->setSize(positions["radius"], 32, 32);

            ogl->addObj(ball, name, "sphere",ball->position.x(), ball->position.y(), ball->position.z(),
                ball->color.x(), ball->color.y(), ball->color.z());
        }
    }
    else if (type == "star") {
        Star* obj = new Star();
        obj->position = { x, y, z };
        obj->color = { colors[0], colors[1], colors[2] };
        obj->setSize(positions["points"], positions["outer"], positions["inner"]);
        ogl->addObj(obj, name, "star",x, y, z, colors[0], colors[1], colors[2]);
    }
    else if (type == "polygon") {
        if (ogl->mode == "2D") {
            Polygon* obj = new Polygon();
            obj->position = { x, y, z };
            obj->color = { colors[0], colors[1], colors[2] };
            obj->setSize(positions["count"], positions["radius"]);
            ogl->addObj(obj, name, "polygon",x, y, z, colors[0], colors[1], colors[2]);
        }
        else {
            Prism* prism = new Prism();
            prism->position = { x,y,z };
            prism->scale =  { 1, 1, 1 } ;
            prism->color = { colors[0], colors[1], colors[2] };
            prism->setSize(positions["count"], positions["radius"], 2.0f);   

            ogl->addObj(prism, name,"prism", prism->position.x(), prism->position.y(), prism->position.z(),
                prism->color.x(), prism->color.y(), prism->color.z());
        }
    }
    else if (type == "line") {
        Line* obj = new Line();
        obj->position = { x, y, z };
        obj->color = { colors[0], colors[1], colors[2] };
        obj->setSize(positions["wL"], positions["x0"], positions["y0"], positions["lineW"]);
        ogl->addObj(obj, name, "line",x, y, z, colors[0], colors[1], colors[2]);
    }
}
void GUI::addObjWindow(const std::string& type, MyOpenGLWidget* ogl) {
    auto colorRGB = std::make_shared<std::array<float, 3>>();
    auto positions = std::make_shared<std::map<std::string, float>>();
    auto fields = std::make_shared<std::map<std::string, QLineEdit*>>();
    auto typeObj = std::make_shared<std::string>();
    

    QWidget *child = new QWidget();
    child->setWindowTitle("Adding object");
    child->resize(450,450);
    child->show();

    QLabel* lbl0 = makeLabel(child,"Enter object name",10,10);
    QLineEdit* name = makeLineEdit(child, "Enter object name", "figure1", 10, 30);

    QLabel* lbl1 = makeLabel(child, "Set X position", 10, 50);
    QLineEdit* xPos = makeLineEdit(child, "Set X position", "0", 10, 70);
    
    QLabel* lbl2 = makeLabel(child, "Set Y position", 10, 90);
    QLineEdit* yPos = makeLineEdit(child, "Set Y position", "0", 10, 110);
    
    QLabel* lbl3 = makeLabel(child, "Set Z position(leave 0 if 2D mode)", 10, 130);
    QLineEdit* zPos = makeLineEdit(child, "Set Z position", "0", 10, 150);

    QPushButton* colorBtn = new QPushButton("Select object color", child);
    colorBtn->move(10,180);
    colorBtn->show();
    QLabel* colorPreview = new QLabel("Color not choiced",child);
    colorPreview->setAlignment(Qt::AlignCenter);
    colorPreview->setStyleSheet("background: #cccccc; padding: 10px;");
    colorPreview->move(10,200);
    colorPreview->show();
    QObject::connect(colorBtn, &QPushButton::clicked, [=]() {
        QColor color = QColorDialog::getColor(Qt::white, child, "Color selection");
        if (color.isValid()) {

            (*colorRGB)[0] = color.redF();
            (*colorRGB)[1] = color.greenF();
            (*colorRGB)[2] = color.blueF();

            QString rgbText = QString("R: %1  G: %2  B: %3")
                .arg(color.red())
                .arg(color.green())
                .arg(color.blue());

            colorPreview->setText(rgbText);       
            colorPreview->setStyleSheet(QString(
                "background: rgb(%1,%2,%3); padding: 10px;")
                .arg(color.red()).arg(color.green()).arg(color.blue()));
        }
        });

    if (type == "rectangle") {
        *typeObj = "rectangle";
        QLabel* lbl4 = makeLabel(child, "Enter width", 10, 240);
        QLineEdit* weight = makeLineEdit(child, "Enter width", "20", 10, 260);
        QLabel* lbl5 = makeLabel(child, "Enter height", 10, 280);
        QLineEdit* height = makeLineEdit(child, "Enter height", "20", 10, 300);
        (*fields)["w"] = weight;
        (*fields)["h"] = height;
    }
    else if (type == "triangle") {
        *typeObj = "triangle";
        QLabel* lbl4 = makeLabel(child, "Enter width", 10, 240);
        QLineEdit* weight = makeLineEdit(child, "Enter width", "7", 10, 260);
        QLabel* lbl5 = makeLabel(child, "Enter height", 10, 280);
        QLineEdit* height = makeLineEdit(child, "Enter height", "7", 10, 300);
        (*fields)["base"] = weight;
        (*fields)["h"] = height;
    }
    else if (type == "circle") {
        *typeObj = "circle";
        QLabel* lbl4 = makeLabel(child, "Enter radius", 10, 240);
        QLineEdit* radius = makeLineEdit(child, "Enter radius", "6", 10, 260);
        (*fields)["radius"] = radius;
    }
    else if (type == "star") {
        *typeObj = "star";
        QLabel* lbl4 = makeLabel(child, "Enter points count", 10, 240);
        QLineEdit* points = makeLineEdit(child, "Enter points count", "6", 10, 260);
        QLabel* lbl5 = makeLabel(child, "Enter outer radius", 10, 280);
        QLineEdit* outer = makeLineEdit(child, "Enter outer radius", "5", 10, 300);
        QLabel* lbl6 = makeLabel(child, "Enter inner radius", 10, 320);
        QLineEdit* inner = makeLineEdit(child, "Enter inner radius", "6", 10, 340);
        (*fields)["points"] = points;
        (*fields)["outer"] = outer;
        (*fields)["inner"] = inner;
    }
    else if (type == "polygon") {
        *typeObj = "polygon";
        QLabel* lbl4 = makeLabel(child, "Enter angles count", 10, 240);
        QLineEdit* angles = makeLineEdit(child, "Enter angles count", "5", 10, 260);
        QLabel* lbl5 = makeLabel(child, "Enter radius", 10, 280);
        QLineEdit* radius = makeLineEdit(child, "Enter radius", "10", 10, 300);
        (*fields)["count"] = angles;
        (*fields)["radius"] = radius;
    }
    else if (type == "line") {
        *typeObj = "line";
        QLabel* lbl4 = makeLabel(child, "Enter line length", 10, 240);
        QLineEdit* weight = makeLineEdit(child, "Enter line length", "10", 10, 260);
        QLabel* lbl5 = makeLabel(child, "Enter start X coordinate", 10, 280);
        QLineEdit* Xcoord = makeLineEdit(child, "Enter start X coordinate", "0", 10, 300);
        QLabel* lbl6 = makeLabel(child, "Enter start Y coordinate", 10, 320);
        QLineEdit* Ycoord = makeLineEdit(child, "Enter start Y coordinate", "0", 10, 340);
        QLabel* lbl7 = makeLabel(child, "Enter line width", 10, 360);
        QLineEdit* lineW = makeLineEdit(child, "Enter line width","3",10, 380);
        (*fields)["wL"] = weight;
        (*fields)["x0"] = Xcoord;
        (*fields)["y0"] = Ycoord;
        (*fields)["lineW"] = lineW;
    }

    QPushButton* create = new QPushButton(child);
    create->setText("Create object");
    create->move(10,400);
    create->show();

    QObject::connect(create, &QPushButton::clicked, [=]()mutable {
        for (auto& p : *fields)
            if (p.first == "count" || p.first == "points" ||
                p.first == "wL" || p.first == "x" || p.first == "y") {
                (*positions)[p.first] = p.second->text().toInt();
            }
            else {
                (*positions)[p.first] = p.second->text().toFloat();
            }
        addObject(*typeObj,name->text().toStdString(),ogl,xPos->text().toFloat(),
            yPos->text().toFloat(), zPos->text().toFloat(),colorRGB->data(), *positions);
    });
}
void GUI::changeMode(const std::string& mode,MyOpenGLWidget* ogl) {
    ogl->setMode(mode);
};
void GUI::movingParcer(MyOpenGLWidget* ogl,const std::string& name,const int& repeatTime,const QStringList& items,
    const std::map<std::string, std::string>& scenarios,const int& speed)
{
    struct Scenario {
        std::string vect;
        int num;
    };
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
void GUI::startMoveObj(MyOpenGLWidget* ogl,const std::string& name,const int& speed,const std::string& vect,const int time,const int to,std::function<void()> onFinished
)
{
    int x = ogl->getX(name);
    int y = ogl->getY(name);
    int z = ogl->getZ(name);

    int step = speed;
    int repeatCount = 0;
    int moved = 0;

    QTimer* timer = new QTimer(ogl); // Родитель виджет, чтобы таймер не удалялся
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



























