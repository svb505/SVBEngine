#include <windows.h>
#include <QPoint>
#include <QMouseEvent>
#include <QWheelEvent>
#include "Camera.h"
#include "Logger.h"
#include <GL/gl.h>
#include <GL/glu.h> 

void Camera::perspective(float fov, float aspect, float zNear, float zFar) {
    float f = 1.0f / tanf(fov * 0.5f * M_PI / 180.0f);
    float mat[16] = {
        f / aspect, 0, 0, 0,
        0, f,        0, 0,
        0, 0,  (zFar + zNear) / (zNear - zFar), -1,
        0, 0, (2 * zFar * zNear) / (zNear - zFar), 0
    };
    glMultMatrixf(mat);
}
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
