#pragma once
#include <GL/glut.h>
#include <GL/gl.h>
#include <QString>
#include <QPainter>
#include <QPointF>

struct TextData {
    QString text;
    float x, y, z;
    float r, g, b;
};

class OpenGLW;
class SceneText {
public:
    QPointF worldToScreen(float x, float y, int widgetWidth, int widgetHeight, OpenGLW* ogl) const;
    void drawGridText(QPainter& painter, float spacing, int count, OpenGLW* ogl);
    void drawText(QPainter& painter, int x, int y, QString text, OpenGLW* ogl);
    void drawText3D(float x, float y, float z, const QString& text, float R = 1.0f, float G = 1.0f, float B = 1.0f);
    void draw3DGridText(float spacing, OpenGLW* ogl);
};
