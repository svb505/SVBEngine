#include <QWidget>
#include <QTimer>
#include <MyOpenGLWidget.h>
#include <Physics.h>


void Phys::movingParcer(MyOpenGLWidget* ogl,const std::string& name,const int& repeatTime,
    const QStringList& items,const std::map<std::string, std::string>& scenarios){
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
            (*queue)[index].vect,
            (*queue)[index].num,
            repeatTime,
            [=]() { (*runNextPtr)(index + 1); }
        );
        };

    (*runNextPtr)(0);
}
void Phys::startMoveObj(MyOpenGLWidget* ogl, const std::string& name, const std::string& vect,
    const int time, const int to, std::function<void()> onFinished) {
    int x = ogl->getX(name);
    int y = ogl->getY(name);
    int z = ogl->getZ(name);

    int step = 5;
    int repeatCount = 0;
    int moved = 0;

    QTimer* timer = new QTimer();
    timer->setInterval(50);

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
        else if (vect == "DLEFTTOP") { x -= currentStep; y += currentStep; }
        else if (vect == "DLEFTBOTTOM") { x -= currentStep; y -= currentStep; }
        else if (vect == "DRIGHTTOP") { x += currentStep; y += currentStep; }
        else if (vect == "DRIGHTBOTTOM") { x += currentStep; y -= currentStep; }

        moved += currentStep;

        ogl->moveObj(name, x, y, z);
        if (moved >= to) {
            moved = 0;
            repeatCount++;
        }
        });

    timer->start();
}