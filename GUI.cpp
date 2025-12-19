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
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <MyOpenGLWidget.h>
#include <QCheckBox>
#include "ImportExport.h"
#include "GUI.h"
#include "ObjectsAction.h"
#include "HUD.h"
#include <format>

void GUI::addContexMenu(QMouseEvent* event, MyOpenGLWidget* ogl, QWidget* parentWindow) {
    LOG_INFO("[GUI] Contex menu showed");
    QMenu menu;
    QMenu* addMenu = menu.addMenu("Add");
    QAction* platformAction = addMenu->addAction("Add platform");
    QAction* rectAction = addMenu->addAction("Add rectangle");
    QAction* pointAction = addMenu->addAction("Add point");
    QAction* circleAction = addMenu->addAction("Add circle");
    QAction* trianglAction = addMenu->addAction("Add triangle");
    QAction* starAction = addMenu->addAction("Add star");
    QAction* polyAction = addMenu->addAction("Add polygon");
    QAction* lineAction = addMenu->addAction("Add line");
    QAction* coneAction = addMenu->addAction("Add cone");
    QAction* cylinderAction = addMenu->addAction("Add cylinder");
    QAction* flatAction = addMenu->addAction("Add flat");
    QAction* changeAction = menu.addAction("Change");
    QAction* removeAction = menu.addAction("Remove");

    QAction* exitAction = menu.addAction("Exit");


    QObject::connect(removeAction, &QAction::triggered, [=]() { openRemoveWindow(ogl); });
    QObject::connect(changeAction, &QAction::triggered, [=]() { openChangeWindow(ogl); });
    QObject::connect(platformAction, &QAction::triggered, [=]() { addObjWindow("platform", ogl); });
    QObject::connect(rectAction, &QAction::triggered, [=]() { addObjWindow("rectangle", ogl); });
    QObject::connect(pointAction, &QAction::triggered, [=]() { addObjWindow("point", ogl); });
    QObject::connect(circleAction, &QAction::triggered, [=]() { addObjWindow("circle", ogl); });
    QObject::connect(trianglAction, &QAction::triggered, [=]() { addObjWindow("triangle", ogl); });
    QObject::connect(starAction, &QAction::triggered, [=]() { addObjWindow("star", ogl); });
    QObject::connect(polyAction, &QAction::triggered, [=]() { addObjWindow("polygon", ogl); });
    QObject::connect(lineAction, &QAction::triggered, [=]() { addObjWindow("line", ogl); });
    QObject::connect(coneAction, &QAction::triggered, [=]() { addObjWindow("cone", ogl); });
    QObject::connect(cylinderAction, &QAction::triggered, [=]() { addObjWindow("cylinder", ogl); });
    QObject::connect(flatAction, &QAction::triggered, [=]() { addObjWindow("flat", ogl); });


    QAction* selectedAction = menu.exec(event->globalPosition().toPoint());

    if (selectedAction == exitAction) {
        if (parentWindow) parentWindow->close();
    }

}
void GUI::aboutWindow() {
    QWidget* Win = new QWidget();
    Win->setWindowTitle("About");
    Win->show();

    QVBoxLayout* layout = new QVBoxLayout(Win);
    QGroupBox* aboutDev = new QGroupBox("About Developper");
    QFormLayout* aboutDevL = new QFormLayout(aboutDev);
    QLabel* infoD = new QLabel("Developper SVB(Svyatoslav) started programming at age 13"
        " in Python and at age 14 in C++.\nMy age when i write this engine is 15."
        " My goal is to become a senior game developer.");

    QGroupBox* aboutSoft = new QGroupBox("About Engine");
    QFormLayout* aboutSoftL = new QFormLayout(aboutSoft);
    QLabel* infoS = new QLabel("SVBEngine is a project for practices my programming skills "
        "and projects for future CV. \nSVBEngine writed full in the C++ and he counted ~3.000 lines of code");


    QGroupBox* links = new QGroupBox("Links");
    QFormLayout* linksL = new QFormLayout(links);
    QLabel* patreon = new QLabel("<a href=\"https://www.patreon.com/c/SVB278\">Donut to Patreon</a>");
    patreon->setTextFormat(Qt::RichText);
    patreon->setTextInteractionFlags(Qt::TextBrowserInteraction);
    patreon->setOpenExternalLinks(true);
    QLabel* boosty = new QLabel("<a href = \https://boosty.to/developsvb\">Donut to Boosty<a/>");
    boosty->setTextFormat(Qt::RichText);
    boosty->setTextInteractionFlags(Qt::TextBrowserInteraction);
    boosty->setOpenExternalLinks(true);
    QLabel* github = new QLabel("<a href=\"https://github.com/svb505\">My GitHub</a>");
    github->setTextFormat(Qt::RichText);
    github->setTextInteractionFlags(Qt::TextBrowserInteraction);
    github->setOpenExternalLinks(true);


    aboutDevL->addRow(infoD);
    aboutSoftL->addRow(infoS);
    linksL->addRow(patreon);
    linksL->addRow(boosty);
    linksL->addRow(github);
    layout->addWidget(aboutDev);
    layout->addWidget(aboutSoft);
    layout->addWidget(links);

}
void GUI::openChangeWindow(MyOpenGLWidget* ogl) {
    auto colorRGB = std::make_shared<std::array<float, 3>>();
    int xTurn = 0;
    int yTurn = 0;
    int zTurn = 0;

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

    QLineEdit* turnX = new QLineEdit();
    turnX->setText("0");
    layout->addRow("New angle turn in X axe", turnX);
    QLineEdit* turnY = new QLineEdit();
    turnY->setText("0");
    layout->addRow("New angle turn in Y axe", turnY);
    QLineEdit* turnZ = new QLineEdit();
    turnZ->setText("0");
    layout->addRow("New angle turn in Z axe", turnZ);

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

        turnX->setText(QString::number(ogl->getTurnX(name)));
        turnY->setText(QString::number(ogl->getTurnY(name)));
        turnZ->setText(QString::number(ogl->getTurnZ(name)));

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
        ogl->changeObj(combo->currentText().toStdString(), xPos->text().toFloat(), yPos->text().toFloat(),
            zPos->text().toFloat(), colorRGB->data(), turnX->text().toInt(), turnY->text().toInt(),
            turnZ->text().toInt()); });

    child->show();
}
void GUI::addMenu(QMainWindow* w, MyOpenGLWidget* ogl) {
    LOG_INFO("[GUI] Menubar added");
    QMenuBar* menubar = w->menuBar();
    QAction* importAction = menubar->addAction("Import");
    QAction* exportAction = menubar->addAction("Export");
    QMenu* modeMenu = menubar->addMenu("Mode");
    QAction* twoD = modeMenu->addAction("Set 2D mode");
    QAction* threeD = modeMenu->addAction("Set 3D mode");
    QMenu* objectsMenu = menubar->addMenu("Objects");
    QMenu* addMenu = objectsMenu->addMenu("Add");
    QAction* platformAction = addMenu->addAction("Add platform");
    QAction* rectAction = addMenu->addAction("Add rectangle");
    QAction* pointAction = addMenu->addAction("Add point");
    QAction* circleAction = addMenu->addAction("Add circle");
    QAction* trianglAction = addMenu->addAction("Add triangle");
    QAction* starAction = addMenu->addAction("Add star");
    QAction* polyAction = addMenu->addAction("Add polygon");
    QAction* lineAction = addMenu->addAction("Add line");
    QAction* coneAction = addMenu->addAction("Add cone");
    QAction* cylinderAction = addMenu->addAction("Add cylinder");
    QAction* flatAction = addMenu->addAction("Add flat");
    QAction* changeAction = objectsMenu->addAction("Change");
    QAction* removeAction = objectsMenu->addAction("Remove");
    QAction* hudAction = menubar->addAction("HUD");
    QAction* sceneAction = menubar->addAction("Scene");
    QAction* scenariosAction = menubar->addAction("Scenarios");
    QAction* cleanAction = menubar->addAction("Clean scene");
    QAction* aboutAction = menubar->addAction("About");


    platformAction->setVisible(ogl->mode == "3D");
    coneAction->setVisible(ogl->mode == "3D");
    cylinderAction->setVisible(ogl->mode == "3D");
    flatAction->setVisible(ogl->mode == "3D");

    QObject::connect(platformAction, &QAction::triggered, [=]() { addObjWindow("platform", ogl); });
    QObject::connect(scenariosAction, &QAction::triggered, [&]() {openScenariosWindow(ogl); });
    QObject::connect(sceneAction, &QAction::triggered, [&]() {openSceneWindow(ogl); });
    QObject::connect(removeAction, &QAction::triggered, [=]() { openRemoveWindow(ogl); });
    QObject::connect(changeAction, &QAction::triggered, [=]() { openChangeWindow(ogl); });
    QObject::connect(rectAction, &QAction::triggered, [=]() { addObjWindow("rectangle", ogl); });
    QObject::connect(pointAction, &QAction::triggered, [=]() { addObjWindow("point", ogl); });
    QObject::connect(circleAction, &QAction::triggered, [=]() { addObjWindow("circle", ogl); });
    QObject::connect(trianglAction, &QAction::triggered, [=]() { addObjWindow("triangle", ogl); });
    QObject::connect(starAction, &QAction::triggered, [=]() { addObjWindow("star", ogl); });
    QObject::connect(polyAction, &QAction::triggered, [=]() { addObjWindow("polygon", ogl); });
    QObject::connect(lineAction, &QAction::triggered, [=]() { addObjWindow("line", ogl); });
    QObject::connect(coneAction, &QAction::triggered, [=]() { addObjWindow("cone", ogl); });
    QObject::connect(cylinderAction, &QAction::triggered, [=]() { addObjWindow("cylinder", ogl); });
    QObject::connect(flatAction, &QAction::triggered, [=]() { addObjWindow("flat", ogl); });
    QObject::connect(hudAction, &QAction::triggered, [=]() { openHudWindow(ogl,ogl->hud); });
    QObject::connect(importAction, &QAction::triggered, [this, ogl]() {ImpExp scene; scene.importSceneWithDialog(ogl); });
    QObject::connect(exportAction, &QAction::triggered, [this, ogl]() {ImpExp scene; scene.exportSceneWithDialog(ogl->getObjects(), ogl); });
    QObject::connect(cleanAction, &QAction::triggered, [this, ogl]() {ogl->clearScene(); });
    QObject::connect(aboutAction, &QAction::triggered, [&]() {aboutWindow(); });
    QObject::connect(twoD, &QAction::triggered, [this, ogl, coneAction, cylinderAction, flatAction, platformAction]() {
        coneAction->setVisible(false);
        cylinderAction->setVisible(false);
        flatAction->setVisible(false);
        platformAction->setVisible(false);
        ogl->setMode("2D");
        });
    QObject::connect(threeD, &QAction::triggered, [this, ogl, coneAction, cylinderAction, flatAction, platformAction]() {
        coneAction->setVisible(true);
        cylinderAction->setVisible(true);
        flatAction->setVisible(true);
        platformAction->setVisible(true);
        ogl->setMode("3D");
        });


};
void GUI::openSceneWindow(MyOpenGLWidget* ogl) {
    auto params = ogl->getProjectionParams();
    QWidget* child = new QWidget();
    child->resize(200, 150);
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
        scenarios.exportScenarios(child, list1);
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
            Action act;
            act.movingParcer(
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

    QObject::connect(btn, &QPushButton::clicked, [combo, ogl]() {
        if (combo->currentIndex() >= 0) {
            ogl->removeObj(combo->currentText().toStdString());
        }
        else {
            QMessageBox::warning(nullptr, "Empty choice", "Choice the object from combobox!");
        }
        });
}
void GUI::addObjWindow(const std::string& type, MyOpenGLWidget* ogl) {
    auto colorRGB = std::make_shared<std::array<float, 3>>();
    auto positions = std::make_shared<std::map<std::string, float>>();
    auto fields = std::make_shared<std::map<std::string, QLineEdit*>>();
    auto designModes = std::make_shared<std::map<std::string, QCheckBox*>>();
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

    if (type == "rectangle" || type == "flat" || type == "platform") {
        *typeObj = type;
        QLineEdit* width = new QLineEdit("Enter width");
        width->setText("20");
        layout->addRow("Enter width", width);
        QLineEdit* height = new QLineEdit("Enter height");
        height->setText("20");
        layout->addRow("Enter height", height);
        if (ogl->mode == "3D") {
            QCheckBox* cb = new QCheckBox("WireFrame mode");
            cb->setChecked(false);
            layout->addRow("Select design mode", cb);
            (*designModes)["m"] = cb;
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
            QCheckBox* cb = new QCheckBox("WireFrame mode");
            cb->setChecked(false);
            layout->addRow("Select design mode", cb);
            (*designModes)["m"] = cb;
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
            QCheckBox* cb = new QCheckBox("WireFrame mode");
            cb->setChecked(false);
            layout->addRow("Select design mode", cb);
            (*designModes)["m"] = cb;
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
            QCheckBox* cb = new QCheckBox("WireFrame mode");
            cb->setChecked(false);
            layout->addRow("Select design mode", cb);
            (*designModes)["m"] = cb;
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
        QCheckBox* cb = new QCheckBox("WireFrame mode");
        cb->setChecked(false);
        layout->addRow("Select design mode", cb);
        (*designModes)["m"] = cb;
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
        QCheckBox* cb = new QCheckBox("WireFrame mode");
        cb->setChecked(false);
        layout->addRow("Select design mode", cb);
        (*designModes)["m"] = cb;
        (*fields)["rB"] = rBottom;
        (*fields)["rT"] = rTop;
        (*fields)["h"] = h;
    }
    else if (type == "point") {
        *typeObj = "point";
        QLineEdit* size = new QLineEdit("Enter point size");
        size->setText("5");
        layout->addRow("Enter size", size);
        (*fields)["size"] = size;
    }
    QLineEdit* turnX = new QLineEdit();
    turnX->setText("0");
    layout->addRow("Angle turn in X axe", turnX);
    (*fields)["turnX"] = turnX;
    QLineEdit* turnY = new QLineEdit();
    turnY->setText("0");
    layout->addRow("Angle turn in Y axe", turnY);
    (*fields)["turnY"] = turnY;
    QLineEdit* turnZ = new QLineEdit();
    turnZ->setText("0");
    layout->addRow("Angle turn in Z axe", turnZ);
    (*fields)["turnY"] = turnZ;


    QPushButton* create = new QPushButton("Create object");
    layout->addRow(create);

    QObject::connect(create, &QPushButton::clicked, [=]()mutable {
        auto objects = ogl->getObjects();
        bool dMode = false;
        for (auto obj : objects) {
            if (obj.first == name->text().toStdString()) {
                LOG_WARN(std::format("Objects with this name '{}' exists!", obj.first));
                QMessageBox::warning(child, "warning", "Objects with this name exists!");
                return;
            }
        }
        for (auto& p : *fields)
            if (p.first == "count" || p.first == "points" ||
                p.first == "wL" || p.first == "x" || p.first == "y" || p.first == "turnX" ||
                p.first == "turnZ") {
                (*positions)[p.first] = p.second->text().toInt();
            }
            else {
                (*positions)[p.first] = p.second->text().toFloat();
            }
        for (auto& md : *designModes) {
            if (md.second->isChecked()) {
                dMode = true;
            }
        }
        Action act;
        act.addObject(*typeObj, name->text().toStdString(), ogl, xPos->text().toFloat(),
            yPos->text().toFloat(), zPos->text().toFloat(), colorRGB->data(), *positions, dMode);
        });
}
void GUI::openHudWindow(MyOpenGLWidget* ogl,HUD* hud) {
    auto cbboxs = std::make_shared<std::map<int, QCheckBox*>>();

    QWidget* child = new QWidget();
    child->resize(200, 150);
    child->setWindowTitle("HUD");
    child->show();

    QFormLayout* layout = new QFormLayout(child);

    for (int i = 0; i < hud->count; i++) {
        QCheckBox* cb = new QCheckBox();
        cb->setChecked(hud->hudTextsVisible[i]);
        (*cbboxs)[i] = cb;
        layout->addRow(QString("Show %1").arg(hud->hudTexts[i]), cb);
    }

    QPushButton* apply = new QPushButton("Apply");
    layout->addRow(apply);

    QObject::connect(apply, &QPushButton::clicked, [ogl,hud,cbboxs]() {
        for (auto check : *cbboxs) {
            if (check.second->isChecked()) hud->hudTextsVisible[check.first] = true;
            else hud->hudTextsVisible[check.first] = false;
            ogl->update();
        }
    });




}