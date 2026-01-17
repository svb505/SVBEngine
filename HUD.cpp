#include "HUD.h"
#include "MyOpenGLWidget.h"
#include <QPainter>

void HUD::drawHud(MyOpenGLWidget* ogl, Camera* camera, GUI* gui) {
    std::vector<QString> args = { 
                                  QString("FPS: %1").arg(ogl->fps),
                                  QString("Objects count: %1").arg(ogl->objects.size()),
                                  QString("Mode: %1").arg(ogl->mode),
                                  QString("Camera incline: %1").arg(camera->camPitch),
                                  QString("Camera horizontal: %1").arg(camera->camYaw),
                                  QString("Camera X offcet: %1").arg(camera->camX),
                                  QString("Camera Y offcet: %1").arg(camera->camY),
                                  QString("Camera distance from center: %1").arg(camera->camDistance),
                                };
    QPainter painter(ogl);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setFont(QFont("Consolas", 11));
    painter.setPen(Qt::white);


    int idx = 0; 
    int x = startX;
    int y = startY;

    while (idx < args.size()) {
        if (hudTextsVisible[idx]) {
            ogl->text->drawText(painter, x, y, args[idx],ogl);
            y -= step;
        } 
        idx++;
    }


    painter.end();
}