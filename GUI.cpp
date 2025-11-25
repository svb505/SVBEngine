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
#include "ImportExport.h"

void GUI::openChangeWindow(MyOpenGLWidget* ogl) {
    auto colorRGB = std::make_shared<std::array<float, 3>>();

    QWidget* child = new QWidget();
    child->setWindowTitle("Changing object");
    child->resize(250, 200);

    QFormLayout* layout = new QFormLayout(child);

    QComboBox* combo = new QComboBox();
    layout->addRow(combo);

    auto objects = ogl->getObjects();
    for (auto& name : objects) {
        combo->addItem(QString::fromStdString(name.first));
    }

    QLabel* lbl1 = new QLabel("Set new X position");
    QLineEdit* xPos = new QLineEdit();
    layout->addRow(lbl1, xPos);

    QLabel* lbl2 = new QLabel("Set new Y position");
    QLineEdit* yPos = new QLineEdit();
    layout->addRow(lbl2, yPos);

    QLabel* lbl3 = new QLabel("Set new Z position");
    QLineEdit* zPos = new QLineEdit();
    layout->addRow(lbl3, zPos);

    QPushButton* colorBtn = new QPushButton("Select new object color");
    QLabel* colorPreview = new QLabel("Color not chosen");
    colorPreview->setAlignment(Qt::AlignCenter);
    layout->addRow(colorBtn);
    layout->addRow(colorPreview);

    auto updateFields = [=]() {
        if (combo->currentIndex() < 0) return;

        std::string name = combo->currentText().toStdString();

        auto objs = ogl->getObjects();
        if (!objs.count(name)) return;

        float x = ogl->getX(name);
        float y = ogl->getY(name);
        float z = ogl->getZ(name);

        xPos->setText(QString::number(x));
        yPos->setText(QString::number(y));
        zPos->setText(QString::number(z));

        auto cols = ogl->getColors(name);

        if (cols.size() >= 3) {
            (*colorRGB)[0] = cols[0];
            (*colorRGB)[1] = cols[1];
            (*colorRGB)[2] = cols[2];

            colorPreview->setText(
                QString("R: %1  G: %2  B: %3")
                .arg(cols[0]).arg(cols[1]).arg(cols[2])
            );

            colorPreview->setStyleSheet(
                QString("background: rgb(%1,%2,%3); padding: 10px;")
                .arg(cols[0]).arg(cols[1]).arg(cols[2])
            );
        }
        };

    QObject::connect(combo,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](int) { updateFields(); }
    );

    if (!objects.empty()) {
        combo->setCurrentIndex(0);
        updateFields();
    }

    QObject::connect(colorBtn, &QPushButton::clicked, [=]() {
        QColor color = QColorDialog::getColor(Qt::white, child);
        if (!color.isValid()) return;

        (*colorRGB)[0] = color.redF();
        (*colorRGB)[1] = color.greenF();
        (*colorRGB)[2] = color.blueF();

        colorPreview->setText(
            QString("R: %1  G: %2  B: %3")
            .arg(color.red()).arg(color.green()).arg(color.blue())
        );

        colorPreview->setStyleSheet(
            QString("background: rgb(%1,%2,%3); padding: 10px;")
            .arg(color.red()).arg(color.green()).arg(color.blue())
        );
        });

    QPushButton* btn = new QPushButton("Change object");
    layout->addRow(btn);

    QObject::connect(btn, &QPushButton::clicked, [=]() {
        if (combo->currentIndex() < 0) return;
        ogl->changeObj(combo->currentText().toStdString(),xPos->text().toFloat(), yPos->text().toFloat(),
            zPos->text().toFloat(),colorRGB->data());});

    child->show();
}
void GUI::addMenu(QMainWindow* w,MyOpenGLWidget* ogl) {
    QMenuBar* menubar = w->menuBar();
    QAction* importAction = menubar->addAction("Import");
    QAction* exportAction = menubar->addAction("Export");
    QMenu* modeMenu = menubar->addMenu("Mode");
    QMenu* objectsMenu = menubar->addMenu("Objects");
    QMenu* addMenu = objectsMenu->addMenu("Add");
    QAction* changeAction = objectsMenu->addAction("Change");
    QAction* removeAction = objectsMenu->addAction("Remove");
    QAction* sceneAction = menubar->addAction("Scene");
    QAction* scenariosAction = menubar->addAction("Scenarios");    
    QAction* cleanAction = menubar->addAction("Clean scene");

    QAction* rectAction = addMenu->addAction("Add rectangle");
    QAction* circleAction = addMenu->addAction("Add circle");
    QAction* trianglAction = addMenu->addAction("Add triangle");
    QAction* starAction = addMenu->addAction("Add star");
    QAction* polyAction = addMenu->addAction("Add polygon");
    QAction* lineAction = addMenu->addAction("Add line");
    QAction* coneAction = addMenu->addAction("Add cone");
    QAction* cylinderAction = addMenu->addAction("Add cylinder");
    QAction* flatAction = addMenu->addAction("Add flat");
    coneAction->setVisible(ogl->mode == "3D");
    cylinderAction->setVisible(ogl->mode == "3D");
    flatAction->setVisible(ogl->mode == "3D");

    QAction* twoD= modeMenu->addAction("Set 2D mode");
    QAction* threeD = modeMenu->addAction("Set 3D mode");
    QObject::connect(removeAction, &QAction::triggered, [&]() {
        openRemoveWindow(ogl);
        });
    QObject::connect(changeAction, &QAction::triggered, [&]() {
        openChangeWindow(ogl);
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
    QObject::connect(coneAction, &QAction::triggered, [&]() {
        addObjWindow("cone", ogl);
        });
    QObject::connect(cylinderAction, &QAction::triggered, [&]() {
        addObjWindow("cylinder", ogl);
        });
    QObject::connect(flatAction, &QAction::triggered, [&]() {
        addObjWindow("flat", ogl);
        });
    QObject::connect(twoD, &QAction::triggered, [this,ogl, coneAction, cylinderAction,flatAction]() {
        coneAction->setVisible(false);
        cylinderAction->setVisible(false);
        flatAction->setVisible(false);
        changeMode("2D",ogl);
        });
    QObject::connect(threeD, &QAction::triggered, [this, ogl, coneAction, cylinderAction,flatAction]() {
        coneAction->setVisible(true);
        cylinderAction->setVisible(true);
        flatAction->setVisible(true);
        changeMode("3D",ogl);
        });
    QObject::connect(importAction, &QAction::triggered, [this, ogl]() {
        ImpExp scene;
        scene.importSceneWithDialog(ogl);
        });
    QObject::connect(exportAction, &QAction::triggered, [this, ogl]() {
        ImpExp scene;
        scene.exportSceneWithDialog(ogl->getObjects(),ogl);
        });
    QObject::connect(cleanAction, &QAction::triggered, [this, ogl]() {
        ogl->clearScene();
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
        ImpExp scenarios;
        scenarios.exportScenarios(child,list1);
        });
    QObject::connect(importBtn, &QPushButton::clicked, [list1, child]() {
        ImpExp scenarios;
        scenarios.importScenarios(child, list1);
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
    if (type == "rectangle" || type == "flat") {
        if (ogl->mode == "2D" || type == "flat") {
            Box* obj = new Box();
            obj->position = { x, y, z };
            obj->color = { colors[0], colors[1], colors[2] };
            obj->setSize(positions["w"], positions["h"]);
            ogl->addObj(obj, name, type,x, y, z, colors[0], colors[1], colors[2]);
        }
        else {
            Cube* cube = new Cube();
            cube->position = { x, y, z };;
            cube->scale = { 1, 1, 1 };       
            cube->rotation = 45.0f;
            cube->color = { colors[0], colors[1], colors[2] };
            cube->setSize(positions["w"], positions["h"], positions["w"]);
            cube->setMode(positions["m"]);
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
            pyr->setMode(positions["m"]);
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
            ball->setMode(positions["m"]);
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
            prism->setMode(positions["m"]);
            prism->setSize(positions["count"], positions["radius"], positions["h"]);

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
    else if (type == "cone") {
        Cone* obj = new Cone();
        obj->position = { x, y, z };
        obj->color = { colors[0], colors[1], colors[2] };
        obj->setSize(positions["r"], positions["h"]);
        obj->setMode(positions["m"]);
        ogl->addObj(obj, name, "cone", x, y, z, colors[0], colors[1], colors[2]);
    }
    else if (type == "cylinder") {
        Cylinder* obj = new Cylinder();
        obj->position = { x, y, z };
        obj->color = { colors[0], colors[1], colors[2] };
        obj->setSize(positions["rT"], positions["rB"], positions["h"]);
        obj->setMode(positions["m"]);
        ogl->addObj(obj, name, "cylinder", x, y, z, colors[0], colors[1], colors[2]);
    }
}
void GUI::addObjWindow(const std::string& type, MyOpenGLWidget* ogl) {
    auto colorRGB = std::make_shared<std::array<float, 3>>();
    auto positions = std::make_shared<std::map<std::string, float>>();
    auto fields = std::make_shared<std::map<std::string, QLineEdit*>>();
    auto typeObj = std::make_shared<std::string>();


    QWidget* child = new QWidget();
    child->setWindowTitle("Adding object");
    child->resize(450, 450);
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

    if (type == "rectangle" || type == "flat") {
        *typeObj = type;
        QLineEdit* width = new QLineEdit("Enter width");
        width->setText("20");
        layout->addRow("Enter width", width);
        QLineEdit* height = new QLineEdit("Enter height");
        height->setText("20");
        layout->addRow("Enter height", height);
        if (ogl->mode == "3D") {
            QLineEdit* mode = new QLineEdit("Enter mode(1.0 - for lines/2.0 - for quads)");
            mode->setText("2.0");
            layout->addRow("Enter mode(1.0 - for lines/2.0 - for quads)", mode);
            (*fields)["m"] = mode;
        }
        (*fields)["w"] = width;
        (*fields)["h"] = height;
    }
    else if (type == "triangle") {
        *typeObj = "triangle";
        QLineEdit* width = new QLineEdit("Enter width");
        width->setText("30");
        layout->addRow("Enter width", width);
        QLineEdit* height = new QLineEdit("Enter height");
        height->setText("30");
        layout->addRow("Enter height", height);
        if (ogl->mode == "3D") {
            QLineEdit* mode = new QLineEdit("Enter mode(1.0 - for lines/2.0 - for quads)");
            mode->setText("2.0");
            layout->addRow("Enter mode(1.0 - for lines/2.0 - for quads)", mode);
            (*fields)["m"] = mode;
        }
        (*fields)["base"] = width;
        (*fields)["h"] = height;
    }
    else if (type == "circle") {
        *typeObj = "circle";
        QLineEdit* radius = new QLineEdit("Enter radius");
        radius->setText("6");
        layout->addRow("Enter radius", radius);
        if (ogl->mode == "3D") {
            QLineEdit* mode = new QLineEdit("Enter mode(1.0 - for lines/2.0 - for quads)");
            mode->setText("2.0");
            layout->addRow("Enter mode(1.0 - for lines/2.0 - for quads)", mode);
            (*fields)["m"] = mode;
        }
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
        layout->addRow("Enter angles count", angles);
        QLineEdit* radius = new QLineEdit("Enter radius");
        radius->setText("10");
        layout->addRow("Enter radius", radius);
        if (ogl->mode == "3D") {
            QLineEdit* h = new QLineEdit("Enter height");
            h->setText("10");
            layout->addRow("Enter height", h);
            QLineEdit* mode = new QLineEdit("Enter mode(1.0 - for lines/2.0 - for quads)");
            mode->setText("2.0");
            layout->addRow("Enter mode(1.0 - for lines/2.0 - for quads)", mode);
            (*fields)["m"] = mode;
            (*fields)["h"] = h;
        }

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
    else if (type == "cone") {
        *typeObj = "cone";
        QLineEdit* radius = new QLineEdit("Enter radius");
        radius->setText("20");
        layout->addRow("Enter width", radius);
        QLineEdit* height = new QLineEdit("Enter height");
        height->setText("20");
        layout->addRow("Enter height", height);
        QLineEdit* mode = new QLineEdit("Enter mode(1.0 - for lines/2.0 - for quads)");
        mode->setText("2.0");
        layout->addRow("Enter mode(1.0 - for lines/2.0 - for quads)", mode);
        (*fields)["m"] = mode;
        (*fields)["r"] = radius;
        (*fields)["h"] = height;

    }
    else if (type == "cylinder") {
        *typeObj = "cylinder";
        QLineEdit* rTop = new QLineEdit("Enter top radius");
        rTop->setText("20");
        layout->addRow("Enter top radius", rTop);
        QLineEdit* rBottom = new QLineEdit("Enter bottom radius");
        rBottom->setText("20");
        layout->addRow("Enter bottom radius", rBottom);
        QLineEdit* h = new QLineEdit("Enter height");
        h->setText("20");
        layout->addRow("Enter height", h);
        QLineEdit* mode = new QLineEdit("Enter mode(1.0 - for lines/2.0 - for quads)");
        mode->setText("2.0");
        layout->addRow("Enter mode(1.0 - for lines/2.0 - for quads)", mode);
        (*fields)["m"] = mode; 
        (*fields)["rB"] = rBottom;
        (*fields)["rT"] = rTop;
        (*fields)["h"] = h;
    }

    QPushButton* create = new QPushButton("Create object");
    layout->addRow(create);

    QObject::connect(create, &QPushButton::clicked, [=]()mutable {
        auto objects = ogl->getObjects();
        for (auto obj : objects) {
            if (obj.first == name->text().toStdString()) {
                QMessageBox::warning(child, "warning", "Objects with this name exists!");
                return;
            }
        }
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



























