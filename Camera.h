#pragma once
#include <QWheelEvent>
#include <QMouseEvent>
#include "HUD.h"

class Camera {
public:
    QPoint lastMouse;
    float camYaw = 0.0f;     // Horizontal rotate
    float camPitch = 25.0f;  // Camera incline
    float camDistance = 400; // Distance from center
    float negativeCamDistance = -camDistance;
    float camX = 0.0f;       // Camera offcet
    float camY = 0.0f;
    void mouseWheel(QMouseEvent* event);
    void changeLastMouse(QMouseEvent* event);
    friend class HUD;
};
