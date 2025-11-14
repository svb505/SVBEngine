#pragma once
#include <QOpenGLFunctions>
#include <QVector3D>
#include <cmath>

class Object : protected QOpenGLFunctions {
public:
    QVector3D position{ 0, 0, 0 };
    QVector3D scale{ 1, 1, 1 };
    float rotation = 0.0f;       
    QVector3D color{ 1, 1, 1 };    

    Object() { initializeOpenGLFunctions(); }
    virtual void render() = 0;  
    virtual ~Object() {}
};
class Box : public Object {
private:
    float width = 1.0f;
    float height = 1.0f;
public:
    void setSize(const float& w,const float& h) {
        width = w;
        height = h;
    }
    void render() override {
        glPushMatrix();
        glTranslatef(position.x(), position.y(), 0);
        glRotatef(rotation, 0, 0, 1);  // вращаем по оси Z
        glScalef(scale.x(), scale.y(), 1);

        glColor3f(color.x(), color.y(), color.z());

        glBegin(GL_QUADS);
        glVertex2f(-width / 2, -height / 2);
        glVertex2f(width / 2, -height / 2);
        glVertex2f(width / 2, height / 2);
        glVertex2f(-width / 2, height / 2);
        glEnd();

        glPopMatrix();
    }
};


class Circle : public Object {
private:
float radius = 0.5f;
public:
    void setRadius(const float& r) { radius = r; }
    void render() override {
        glPushMatrix();
        glTranslatef(position.x(), position.y(), 0);
        glRotatef(rotation, 0, 0, 1);
        glScalef(scale.x(), scale.y(), 1);

        glColor3f(color.x(), color.y(), color.z());

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0, 0); 
        const int segments = 40;
        for (int i = 0; i <= segments; ++i) {
            float angle = i * 2 * M_PI / segments;
            float x = radius * cos(angle);
            float y = radius * sin(angle);
            glVertex2f(x, y);
        }
        glEnd();

        glPopMatrix();
    }
};
