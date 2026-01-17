#pragma once
#include <GL/glut.h>
#include <GL/gl.h>
#include <QString>
#include <QPainter>
#include <QPointF>

class MyOpenGLWidget;
class SceneText {
public:
    QPointF worldToScreen(float x, float y, int widgetWidth, int widgetHeight, MyOpenGLWidget* ogl) const;
    void drawGridText(QPainter& painter, float spacing, int count, MyOpenGLWidget* ogl);
    void drawText(QPainter& painter, int x, int y, QString text, MyOpenGLWidget* ogl);
    void drawText3D(float x, float y, float z, const QString& text, float R = 1.0f, float G = 1.0f, float B = 1.0f);
    void draw3DGridText(float spacing, MyOpenGLWidget* ogl);
};
