#include <QPoint>
#include <QMouseEvent>
#include <QWheelEvent>
#include "Camera.h"
#include "Logger.h"

void Camera::mouseWheel(QMouseEvent* event) {
    QPoint d = event->pos() - lastMouse;
    lastMouse = event->pos();

    if (event->buttons() & Qt::RightButton)
    {
        camYaw += d.x() * 0.3f;
        camPitch += d.y() * 0.3f;

        if (camPitch > 89) camPitch = 89;
        if (camPitch < -89) camPitch = -89;
    }

    if (event->buttons() & Qt::LeftButton)
    {
        float panSpeed = camDistance * 0.002f;

        camX += -d.x() * panSpeed;
        camY += d.y() * panSpeed;
    }

}
void Camera::changeLastMouse(QMouseEvent* event) {
    lastMouse = event->pos();
}
