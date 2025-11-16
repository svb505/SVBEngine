#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>
#include <QTimer>
#include <QLineEdit>
#include <QColorDialog>
#include <QCombobox>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QListWidget>
#include <GUI.h>
#include <MyOpenGLWidget.h>

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
    QAction* physicsAction = menubar->addAction("Physics simulation");
    

    QAction* rectAction = addMenu->addAction("Add rectangle");
    QAction* circleAction = addMenu->addAction("Add circle");
    QAction* trianglAction = addMenu->addAction("Add triangle");
    QAction* starAction = addMenu->addAction("Add star");
    QAction* polyAction = addMenu->addAction("Add polygon");
    QAction* lineAction = addMenu->addAction("Add line");

    QAction* twoD= modeMenu->addAction("Set 2D mode");
    QAction* threeD = modeMenu->addAction("Set 3D mode");

    QObject::connect(physicsAction, &QAction::triggered, [&]() {
        openPhysicsWindow();
    });
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
};
void GUI::openPhysicsWindow() {
    //
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
    std::vector<std::string> scenarios = { "Moving to left" ,"Moving to right","Moving to top",
        "Moving to bottom","Moving to diagonal(left -> top)","Moving to diagonal(left -> bottom)",
        "Moving to diagonal(right -> top)","Moving to diagonal(right -> bottom)"};

    QWidget* child = new QWidget();
    child->resize(500, 400);
    child->setWindowTitle("Scenarios");
    child->show();

    QLabel* lbl = makeLabel(child, "Select object", 10, 10);

    QComboBox* combo = new QComboBox(child);
    combo->move(10, 30);
    for (auto name : ogl->getObjects()) {
        combo->addItem(QString::fromStdString(name.first));
    }
    combo->show();

    QLabel* lbl1 = makeLabel(child, "Select scenario", 40, 10);

    QComboBox* combo1 = new QComboBox(child);
    combo1->move(40, 30);
    for (std::string name : scenarios) {
        combo1->addItem(QString::fromStdString(name));
    }
    combo1->show();

    QListWidget* list = new QListWidget(child);
    list->move(40, 50);
    list->resize(160, 100);

    QPushButton* add = new QPushButton(child);
    add->setText("Add scenario");
    add->move(10,50);
    add->show();

    QObject::connect(add, &QPushButton::clicked, [&]() {
        QString item = combo->currentText();
        list->addItem(item);
    });

    QLabel* lbl2 = makeLabel(child, "How many times to repeat", 10, 10);
    QComboBox* combo2 = new QComboBox(child);
    combo2->move(40, 60);
    combo2->addItem("1 times");
    combo2->addItem("2 times");
    combo2->addItem("5 times");
    combo2->addItem("10 times");
    combo2->addItem("constantly");
    combo2->show();


    QPushButton* start = new QPushButton(child);
    start->setText("Start");
    start->move(10, 700);
    start->show();






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
        Box* obj = new Box();
        obj->position = { x, y, z };
        obj->color = { colors[0], colors[1], colors[2] };
        obj->setSize(positions["w"], positions["h"]);
        ogl->addObj(obj, name, x,y,z, colors[0], colors[1], colors[2]);
    }
    else if (type == "triangle") {
        Triangle* obj = new Triangle();
        obj->position = { x, y, z };
        obj->color = { colors[0], colors[1], colors[2] };
        obj->setSize(positions["base"], positions["h"]);
        ogl->addObj(obj, name, x, y, z, colors[0], colors[1], colors[2]);
    }
    else if (type == "circle") {
        Circle* obj = new Circle();
        obj->position = { x, y, z };
        obj->color = { colors[0], colors[1], colors[2] };
        obj->setRadius(positions["radius"]);
        ogl->addObj(obj, name, x, y, z, colors[0], colors[1], colors[2]);
    }
    else if (type == "star") {
        Star* obj = new Star();
        obj->position = { x, y, z };
        obj->color = { colors[0], colors[1], colors[2] };
        obj->setSize(positions["points"], positions["outer"], positions["inner"]);
        ogl->addObj(obj, name, x, y, z, colors[0], colors[1], colors[2]);
    }
    else if (type == "polygon") {
        Polygon* obj = new Polygon();
        obj->position = { x, y, z };
        obj->color = { colors[0], colors[1], colors[2] };
        obj->setSize(positions["count"], positions["radius"]);
        ogl->addObj(obj, name, x, y, z, colors[0], colors[1], colors[2]);
    }
    else if (type == "line") {
        Line* obj = new Line();
        obj->position = { x, y, z };
        obj->color = { colors[0], colors[1], colors[2] };
        obj->setSize(positions["wL"], positions["x0"], positions["y0"], positions["lineW"]);
        ogl->addObj(obj, name, x, y, z, colors[0], colors[1], colors[2]);
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
    QLineEdit* yPos = makeLineEdit(child, "Set Y position", "3", 10, 110);
    
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