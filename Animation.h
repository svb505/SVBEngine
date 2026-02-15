#pragma once
#include <QObject>
#include <QTimer>
#include <string>

class OpenGLW;

class AnimationSystem : public QObject {
    Q_OBJECT

private:
    int animTargetX = 0;
    int animSpeed = 5;
    std::string animName;

    OpenGLW* oglPtr = nullptr;
    QTimer* animTimer = nullptr;

private slots:
    void animateMove(); 

public:
    explicit AnimationSystem(OpenGLW* ogl, QObject* parent = nullptr);
    void startMove(const std::string& name, int targetX, int speed);
};
