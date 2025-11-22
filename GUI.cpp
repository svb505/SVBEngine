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
#include <QFormLayout>

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

    QFormLayout* layout = new QFormLayout(child);

    std::string lR = std::format("X from {:.1f}; X to {:.1f}", params.left, params.right);
    std::string bT = std::format("Y from {:.1f}; Y to {:.1f}", params.bottom, params.top);
    std::string znZF = std::format("Z from {:.1f}; Z to {:.1f}", params.zNear, params.zFar);

    QLabel* infoScene = new QLabel("Scene info");
    layout->addRow(infoScene);
    QLabel* infoX = new QLabel(QString::fromStdString(lR));
    layout->addRow(infoX);
    QLabel* infoY = new QLabel(QString::fromStdString(bT));
    layout->addRow(infoY);
    QLabel* infoZ = new QLabel(QString::fromStdString(znZF));
    layout->addRow(infoZ);
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

    QFormLayout* layout = new QFormLayout(child);

    QLabel* lbl1 = new QLabel("Select object");
    layout->addRow(lbl1);

    QComboBox* combo1 = new QComboBox();
    for (auto name : ogl->getObjects()) {
        combo1->addItem(QString::fromStdString(name.first));
    }
    layout->addRow(combo1);

    QLabel* pix = new QLabel("Move on ... pixels");
    layout->addRow(pix);
    QLineEdit* pixels = new QLineEdit("50");
    layout->addRow(pixels);

    QLabel* lbl2 = new QLabel("Select scenario");
    layout->addRow(lbl2);

    QComboBox* combo2 = new QComboBox();
    for (auto name : scenarios) {
        combo2->addItem(QString::fromStdString(name.second));
    }
    layout->addRow(combo2);

    QLabel* lbl3 = new QLabel("Added scenarios");
    layout->addRow(lbl3);
    QListWidget* list1 = new QListWidget();
    list1->resize(190, 100);
    layout->addRow(list1);

    QPushButton* add = new QPushButton("Add scenario");
    layout->addRow(add);

    QObject::connect(add, &QPushButton::clicked, [combo2, list1, pixels]() {
        QString item = combo2->currentText();
        bool ok;
        int pix = pixels->text().toInt(&ok);
        if (!ok) pix = 0;

        QString str = QString("%1 %2").arg(item).arg(pix);
        list1->addItem(str);
        });

    QPushButton* exportBtn = new QPushButton("Export scenarios");
    layout->addRow(exportBtn);

    QPushButton* importBtn = new QPushButton("Import scenarios");
    layout->addRow(importBtn);

    QObject::connect(exportBtn, &QPushButton::clicked, [list1, child]() {
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
        });
    QObject::connect(importBtn, &QPushButton::clicked, [list1, child]() {
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
        });


    QLabel* lbl4 = new QLabel("How many times to repeat");
    layout->addRow(lbl4);
    QComboBox* combo3 = new QComboBox();
    combo3->addItem("1");
    combo3->addItem("2");
    combo3->addItem("5");
    combo3->addItem("10");
    layout->addRow(combo3);

    QLabel* lbl5 = new QLabel("Enter speed");
    layout->addRow(lbl5);
    QLineEdit* speed = new QLineEdit("Enter speed");
    speed->setText("20");
    layout->addRow(speed);

    QPushButton* start = new QPushButton("Start");
    layout->addRow(start);

    QObject::connect(start, &QPushButton::clicked,
        [this, ogl, combo1, combo3, list1, scenarios, pixels, speed]() {
            QStringList texts;
            for (int i = 0; i < list1->count(); i++) {
                texts << list1->item(i)->text();
            }
            movingParcer(
                ogl,
                combo1->currentText().toStdString(),
                combo3->currentText().toInt(),
                texts,
                scenarios,
                speed->text().toInt()
            );
        });
}
void GUI::openRemoveWindow(MyOpenGLWidget* ogl) {
    QWidget* child = new QWidget();
    child->setWindowTitle("Removing object");
    child->resize(250, 150);
    child->show();
    QFormLayout* layout = new QFormLayout(child);

    QLabel* lbl = new QLabel("Avaibles objects");
    layout->addRow(lbl);

    QComboBox* combo = new QComboBox();
    for (auto name : ogl->getObjects()) {
        combo->addItem(QString::fromStdString(name.first));
    }
    layout->addRow(combo);
    
    
    QPushButton* btn = new QPushButton("Remove object");
    layout->addRow(btn);

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
    QFormLayout* layout = new QFormLayout(child);

    QLabel* lbl0 = new QLabel("Enter object name");
    layout->addRow(lbl0);
    QLineEdit* name = new QLineEdit("Enter object name");
    name->setText("figure1");
    layout->addRow(name);
    QLabel* lbl1 = new QLabel("Set X position");
    layout->addRow(lbl1);
    QLineEdit* xPos = new QLineEdit("Set X position");
    xPos->setText("0");
    layout->addRow(xPos);
    
    QLabel* lbl2 = new QLabel("Set Y position");
    layout->addRow(lbl2);
    QLineEdit* yPos = new QLineEdit("Set Y position");
    yPos->setText("0");
    layout->addRow(yPos);
    
    QLabel* lbl3 = new QLabel("Set Z position(leave 0 if 2D mode)");
    layout->addRow(lbl3);
    QLineEdit* zPos = new QLineEdit("Set Z position");
    zPos->setText("0");
    layout->addRow(zPos);

    QPushButton* colorBtn = new QPushButton("Select object color");
    layout->addRow(colorBtn);
    QLabel* colorPreview = new QLabel("Color not choiced");
    colorPreview->setAlignment(Qt::AlignCenter);
    colorPreview->setStyleSheet("background: #cccccc; padding: 10px;");
    layout->addRow(colorPreview);
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
        QLineEdit* width = new QLineEdit("Enter width");
        width->setText("20");
        layout->addRow("Enter width",width);
        QLineEdit* height = new QLineEdit("Enter height");
        height->setText("20");
        layout->addRow("Enter height",height);
        (*fields)["w"] = width;
        (*fields)["h"] = height;
    }
    else if (type == "triangle") {
        *typeObj = "triangle";
        QLineEdit* weight = new QLineEdit("Enter width");
        weight->setText("7");
        layout->addRow("Enter width",weight);
        QLineEdit* height = new QLineEdit("Enter height");
        height->setText("7");
        layout->addRow("Enter height", weight);
        (*fields)["base"] = weight;
        (*fields)["h"] = height;
    }
    else if (type == "circle") {
        *typeObj = "circle";
        QLineEdit* radius = new QLineEdit("Enter radius");
        radius->setText("6");
        layout->addRow("Enter radius", radius);
        (*fields)["radius"] = radius;
    }
    else if (type == "star") {
        *typeObj = "star";
        QLineEdit* points = new QLineEdit("Enter points count");
        points->setText("6");
        layout->addRow("Enter points count", points);
        QLineEdit* outer = new QLineEdit("Enter outer radius");
        outer->setText("5");
        layout->addRow("Enter outer radius", outer);
        QLineEdit* inner = new QLineEdit("Enter inner radius");
        inner->setText("6");
        layout->addRow("Enter inner radius", inner);
        (*fields)["points"] = points;
        (*fields)["outer"] = outer;
        (*fields)["inner"] = inner;
    }
    else if (type == "polygon") {
        *typeObj = "polygon";
        QLineEdit* angles = new QLineEdit("Enter angles count");
        angles->setText("5");
        layout->addRow("Enter angles count",angles);
        QLineEdit* radius = new QLineEdit("Enter radius");
        radius->setText("10");
        layout->addRow("Enter radius",radius);
        (*fields)["count"] = angles;
        (*fields)["radius"] = radius;
    }
    else if (type == "line") {
        *typeObj = "line";
        QLineEdit* len = new QLineEdit("Enter line length");
        len->setText("10");
        layout->addRow("Enter line length", len);
        QLineEdit* Xcoord = new QLineEdit("Enter start X coordinate");
        Xcoord->setText("0");
        layout->addRow("Enter start X coordinate", Xcoord);
        QLineEdit* Ycoord = new QLineEdit("Enter start Y coordinate");
        Ycoord->setText("0");
        layout->addRow("Enter start Y coordinate", Ycoord);
        QLineEdit* lineW = new QLineEdit("Enter line width");
        lineW->setText("3");
        layout->addRow("Enter line width", lineW);
        (*fields)["wL"] = len;
        (*fields)["x0"] = Xcoord;
        (*fields)["y0"] = Ycoord;
        (*fields)["lineW"] = lineW;
    }

    QPushButton* create = new QPushButton("Create object");
    layout->addRow(create);

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
void GUI::startMoveObj(MyOpenGLWidget* ogl,const std::string& name,const int& speed,const std::string& vect,const int time,const int to,std::function<void()> onFinished)
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



























