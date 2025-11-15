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
void GUI::openRemoveWindow(MyOpenGLWidget* ogl) {
    QWidget* child = new QWidget();
    child->setWindowTitle("Removing object");
    child->resize(250, 150);
    child->show();

    QLabel* lbl = new QLabel(child);
    lbl->setText("Avaibles objects");
    lbl->move(10, 10);
    lbl->show();

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
    const  float& y, const  float& z,
    float colors[],std::map<std::string,float>& positions) {
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
    std::map<std::string, float> positions;
    std::map<std::string, QLineEdit*> fields;
    std::string typeObj;

    QWidget *child = new QWidget();
    child->setWindowTitle("Adding object");
    child->resize(450,450);
    child->show();

    QLabel* lbl0 = new QLabel(child);
    lbl0->setText("Enter object name");
    lbl0->move(10,10);
    lbl0->show();
    QLineEdit* name = new QLineEdit(child);
    name->setPlaceholderText("Enter object name");
    name->move(10,30);
    name->show();
    name->setText("figure1");

    QLabel* lbl1 = new QLabel(child);
    lbl1->setText("Set X position");
    lbl1->move(10,50);
    lbl1->show();
    QLineEdit* xPos = new QLineEdit(child);
    xPos->setPlaceholderText("Set X position");
    xPos->move(10, 70);
    xPos->setText("0");
    xPos->show();
    

    QLabel* lbl2 = new QLabel(child);
    lbl2->setText("Set Y position");
    lbl2->move(10, 90);
    lbl2->show();
    QLineEdit* yPos = new QLineEdit(child);
    yPos->setPlaceholderText("Set Y position");
    yPos->move(10, 110);
    yPos->setText("0");
    yPos->show();
    

    QLabel* lbl3 = new QLabel(child);
    lbl3->setText("Set Z position(leave 0 if 2D mode)");
    lbl3->move(10, 130);
    lbl3->show();
    QLineEdit* zPos = new QLineEdit(child);
    zPos->setPlaceholderText("Set Z position");
    zPos->move(10, 150);
    zPos->setText("0");
    zPos->show();

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
        typeObj = "rectangle";
        QLabel* lbl4 = new QLabel(child);
        lbl4->setText("Enter width");
        lbl4->move(10, 240);
        lbl4->show();
        QLineEdit* weight = new QLineEdit(child);
        weight->setPlaceholderText("Enter width");
        weight->move(10, 260);
        weight->setText("10");
        weight->show();
        QLabel* lbl5 = new QLabel(child);
        lbl5->setText("Enter height");
        lbl5->move(10, 280);
        lbl5->show();
        QLineEdit* height = new QLineEdit(child);
        height->setPlaceholderText("Enter height");
        height->move(10, 300);
        height->setText("10");
        height->show();
        fields["w"] = weight;
        fields["h"] = height;
    }
    else if (type == "triangle") {
        typeObj = "triangle";
        QLabel* lbl4 = new QLabel(child);
        lbl4->setText("Enter width");
        lbl4->move(10, 240);
        lbl4->show();
        QLineEdit* weight = new QLineEdit(child);
        weight->setPlaceholderText("Enter width");
        weight->move(10, 260);
        weight->setText("5");
        weight->show();
        QLabel* lbl5 = new QLabel(child);
        lbl5->setText("Enter height");
        lbl5->move(10, 280);
        lbl5->show();
        QLineEdit* height = new QLineEdit(child);
        height->setPlaceholderText("Enter height");
        height->move(10, 300);
        height->setText("7");
        height->show();
        fields["base"] = weight;
        fields["h"] = height;
    }
    else if (type == "circle") {
        typeObj = "circle";
        QLabel* lbl4 = new QLabel(child);
        lbl4->setText("Enter radius");
        lbl4->move(10, 240);
        lbl4->show();
        QLineEdit* radius = new QLineEdit(child);
        radius->setPlaceholderText("Enter radius");
        radius->move(10, 260);
        radius->setText("6");
        radius->show();
        fields["radius"] = radius;
    }
    else if (type == "star") {
        typeObj = "star";
        QLabel* lbl4 = new QLabel(child);
        lbl4->setText("Enter points count");
        lbl4->move(10, 240);
        lbl4->show();
        QLineEdit* points = new QLineEdit(child);
        points->setPlaceholderText("Enter points count");
        points->move(10, 260);
        points->setText("6");
        points->show();
        QLabel* lbl5 = new QLabel(child);
        lbl5->setText("Enter outer radius");
        lbl5->move(10, 280);
        lbl5->show();
        QLineEdit* outer = new QLineEdit(child);
        outer->setPlaceholderText("Enter outer radius");
        outer->move(10, 300);
        outer->setText("5");
        outer->show();
        QLabel* lbl6 = new QLabel(child);
        lbl6->setText("Enter inner radius");
        lbl6->move(10, 320);
        lbl6->show();
        QLineEdit* inner = new QLineEdit(child);
        inner->setPlaceholderText("Enter inner radius");
        inner->move(10, 340);
        inner->setText("6");
        inner->show();
        fields["points"] = points;
        fields["outer"] = outer;
        fields["inner"] = inner;
    }
    else if (type == "polygon") {
        typeObj = "polygon";
        QLabel* lbl4 = new QLabel(child);
        lbl4->setText("Enter angles count");
        lbl4->move(10, 240);
        lbl4->show();
        QLineEdit* angles = new QLineEdit(child);
        angles->setPlaceholderText("Enter angles count");
        angles->move(10, 260);
        angles->setText("5");
        angles->show();
        QLabel* lbl5 = new QLabel(child);
        lbl5->setText("Enter radius");
        lbl5->move(10, 280);
        lbl5->show();
        QLineEdit* radius = new QLineEdit(child);
        radius->setPlaceholderText("Enter radius");
        radius->move(10, 300);
        radius->setText("10");
        radius->show();
        fields["count"] = angles;
        fields["radius"] = radius;
    }
    else if (type == "line") {
        typeObj = "line";
        QLabel* lbl4 = new QLabel(child);
        lbl4->setText("Enter line width");
        lbl4->move(10, 240);
        lbl4->show();
        QLineEdit* weight = new QLineEdit(child);
        weight->setPlaceholderText("Enter line width");
        weight->move(10, 260);
        weight->setText("10");
        weight->show();
        QLabel* lbl5 = new QLabel(child);
        lbl5->setText("Enter start X coordinate");
        lbl5->move(10, 280);
        lbl5->show();
        QLineEdit* Xcoord = new QLineEdit(child);
        Xcoord->setPlaceholderText("Enter start X coordinate");
        Xcoord->move(10, 300);
        Xcoord->setText("0");
        Xcoord->show();
        QLabel* lbl6 = new QLabel(child);
        lbl6->setText("Enter start Y coordinate");
        lbl6->move(10, 320);
        lbl6->show();
        QLineEdit* Ycoord = new QLineEdit(child);
        Ycoord->setPlaceholderText("Enter start Y coordinate");
        Ycoord->move(10, 340);
        Ycoord->setText("0");
        Ycoord->show();
        QLabel* lbl7 = new QLabel(child);
        lbl7->setText("Enter line width");
        lbl7->move(10, 360);
        lbl7->show();
        QLineEdit* lineW = new QLineEdit(child);
        lineW->setPlaceholderText("Enter line width");
        lineW->move(10, 380);
        lineW->setText("3");
        lineW->show();
        fields["wL"] = weight;
        fields["x0"] = Xcoord;
        fields["y0"] = Ycoord;
        fields["lineW"] = lineW;
    }

    QPushButton* create = new QPushButton(child);
    create->setText("Create object");
    create->move(10,400);
    create->show();

    QObject::connect(create, &QPushButton::clicked, [=]()mutable {
        for (auto& p : fields)
            if (p.first == "count" || p.first == "points" ||
                p.first == "wL" || p.first == "x" || p.first == "y") {
                positions[p.first] = p.second->text().toInt();
            }
            else {
                positions[p.first] = p.second->text().toFloat();
            }
        addObject(typeObj,name->text().toStdString(),ogl,xPos->text().toFloat(),
            yPos->text().toFloat(), zPos->text().toFloat(),colorRGB->data(), positions);
    });
}
void GUI::changeMode(const std::string& mode,MyOpenGLWidget* ogl) {
    ogl->setMode(mode);
};