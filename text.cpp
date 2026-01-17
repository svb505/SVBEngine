#include "text.h"
#include "MyOpenGLWidget.h"


QPointF SceneText::worldToScreen(float x, float y, int widgetWidth, int widgetHeight, MyOpenGLWidget* ogl) const {
    float sx = (x - ogl->left) / (ogl->right - ogl->left) * widgetWidth;
    float sy = widgetHeight - (y - ogl->bottom) / (ogl->top - ogl->bottom) * widgetHeight;
    return QPointF(sx, sy);
}
void SceneText::drawGridText(QPainter& painter, float spacing, int count, MyOpenGLWidget* ogl) {
    for (int x = int(ogl->left); x < int(ogl->right); x += int(spacing))
        drawText(painter, x, 0, QString::number(x),ogl);

    for (int y = int(ogl->bottom); y < int(ogl->top); y += int(spacing))
        if (y != 3) {
            drawText(painter, 0, y, QString::number(y),ogl);
        }
}
void SceneText::drawText(QPainter& painter, int x, int y, QString text, MyOpenGLWidget* ogl) {
    QPointF p = worldToScreen(x + 0.1, y + 0.2, ogl->width(), ogl->height(),ogl);
    painter.drawText(p, text);
}
void SceneText::drawText3D(float x, float y, float z, const QString& text, float R, float G, float B)
{
    glColor3f(R, G, B);
    glRasterPos3f(x, y, z);

    for (QChar c : text)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c.toLatin1());
}
void SceneText::draw3DGridText(float spacing, MyOpenGLWidget* ogl) {
    for (int x = int(ogl->left); x < int(ogl->right); x += int(spacing))
        drawText3D(x, 1.0f, 0.0f, QString::number(x));

    for (int y = int(ogl->bottom); y < int(ogl->top); y += int(spacing))
        if (y != 3) {
            drawText3D(0.0f, y, 0.0f, QString::number(y));
        }
    for (int z = int(ogl->zNear); z < int(ogl->zFar); z += int(spacing))
        drawText3D(0.0f, 1.0f, z, QString::number(z));
}