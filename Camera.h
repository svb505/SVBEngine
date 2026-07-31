#pragma once
#include <QWheelEvent>
#include <QMouseEvent>
#include "HUD.h"

class Camera {
private:
    QPoint lastMouse;
    float camYaw = 0.0f;     
    float camPitch = 25.0f; 
    float camDistance = 400; // Distance from center
    float negativeCamDistance = -camDistance;
    float camX = 0.0f;       
    float camY = 0.0f;
    bool cameraFix = false;
public:
    void setCameraFix(bool arg);

    bool getCameraFix();
    float getCamX();
    float getCamY();
    float getCamPitch();
    float getCamYaw();
    float getNegativeCamDist();

    void mouseWheel(QMouseEvent* event);
    void changeLastMouse(QMouseEvent* event);
    void perspective(float fov, float aspect, float zNear, float zFar);
    friend class HUD;
};
