#include "Animation.h"
#include "OpenGLW.h"

AnimationSystem::AnimationSystem(OpenGLW* ogl, QObject* parent): QObject(parent), oglPtr(ogl){
    animTimer = new QTimer(this);

    connect(animTimer,
        &QTimer::timeout,
        this,
        &AnimationSystem::animateMove);
}

void AnimationSystem::animateMove()
{
    auto& data = oglPtr->objects[animName];

    if (data.x < animTargetX)
        data.x += animSpeed;
    else {
        animTimer->stop();
        return;
    }

    oglPtr->update();
}

void AnimationSystem::startMove(const std::string& name, int targetX, int speed)
{
    animName = name;
    animTargetX = targetX;
    animSpeed = speed;
    animTimer->start(16);
}
