#pragma once
#include <QOpenGLFunctions>
#include <QVector3D>
#include <cmath>

/*
 * БАЗОВЫЙ КЛАСС Object
 * --------------------
 * Это основа всех фигур (2D и 3D).
 * У каждой есть позиция, масштаб, цвет, угол вращения.
 */
class Object : protected QOpenGLFunctions {
public:
    QVector3D position{ 0, 0, 0 }; // x, y, z (z позже пригодится для 3D)
    QVector3D scale{ 1, 1, 1 };
    float rotation = 0.0f;       // угол поворота в градусах
    QVector3D color{ 1, 1, 1 };    // цвет RGB

    Object() { initializeOpenGLFunctions(); }
    virtual void render() = 0;   // чисто виртуальная функция (должна быть у каждой фигуры)
    virtual ~Object() {}
};


/*
 * КЛАСС Box — квадрат/прямоугольник
 * --------------------------------
 * Самая базовая фигура 2D.
 */
class Box : public Object {
public:
    float width = 1.0f;
    float height = 1.0f;

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


/*
 * КЛАСС Circle — окружность
 * -------------------------
 * Использует простую аппроксимацию тригонометрией.
 */
class Circle : public Object {
public:
    float radius = 0.5f;

    void render() override {
        glPushMatrix();
        glTranslatef(position.x(), position.y(), 0);
        glRotatef(rotation, 0, 0, 1);
        glScalef(scale.x(), scale.y(), 1);

        glColor3f(color.x(), color.y(), color.z());

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0, 0); // центр
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
