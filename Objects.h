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
    float x() const { return position.x(); }
    float y() const { return position.y(); }
    float z() const { return position.z(); }
    virtual ~Object() {}
};

class Box : public Object {
private:
    float width = 1.0f;
    float height = 1.0f;
public:
    float getSize() { return width; return height; }
    void setSize(const float& w,const float& h) {
        width = w;
        height = h;
    }
    void render() override {
        glPushMatrix();
        glTranslatef(position.x(), position.y(), 0);
        glRotatef(rotation, 0, 0, 1);  
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
    float getRadius() { return radius; }
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
class Triangle : public Object {
private:
    float base = 1.0f;
    float height = 1.0f;
public:
    float getSize() { return base; return height; }
    void setSize(const float& b, const float& h) {
        base = b;
        height = h;
    }

    void render() override {
        glPushMatrix();
        glTranslatef(position.x(), position.y(), 0);
        glRotatef(rotation, 0, 0, 1);  
        glScalef(scale.x(), scale.y(), 1);

        glColor3f(color.x(), color.y(), color.z());

        glBegin(GL_TRIANGLES);
        glVertex2f(-base / 2, -height / 2);
        glVertex2f(base / 2, -height / 2);  
        glVertex2f(0, height / 2);          
        glEnd();

        glPopMatrix();
    }
};
class Polygon : public Object {
private:
    int sides = 0;
    float radius = 1.0f;  
public:
    void setSize(const int& s, const float& r) {
        sides = s;
        radius = r;
    }
    void render() override {
        glPushMatrix();
        glTranslatef(position.x(), position.y(), 0);
        glRotatef(rotation, 0, 0, 1);
        glScalef(scale.x(), scale.y(), 1);

        glColor3f(color.x(), color.y(), color.z());

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0, 0); 
        for (int i = 0; i <= sides; ++i) {
            float angle = 2.0f * M_PI * i / sides;
            float x = cos(angle) * radius;
            float y = sin(angle) * radius;
            glVertex2f(x, y);
        }
        glEnd();


        glPopMatrix();
    }
};
class Star : public Object {
private:
    int points = 5;      
    float outerRadius = 1.0f;
    float innerRadius = 0.5f;
public:
    void setSize(const int& p,const float& outer, const float& inner) {
        points = p;
        outerRadius = outer;
        innerRadius = inner;
    }
    void render() override {
        glPushMatrix();
        glTranslatef(position.x(), position.y(), 0);
        glRotatef(rotation, 0, 0, 1);
        glScalef(scale.x(), scale.y(), 1);

        glColor3f(color.x(), color.y(), color.z());

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0, 0); // центр

        for (int i = 0; i <= points; ++i) {
            float outerAngle = 2.0f * M_PI * i / points;
            float innerAngle = outerAngle + M_PI / points;

            glVertex2f(cos(outerAngle) * outerRadius, sin(outerAngle) * outerRadius);
            glVertex2f(cos(innerAngle) * innerRadius, sin(innerAngle) * innerRadius);
        }

        glEnd();
        glPopMatrix();
    }
};
class Line : public Object {
private:
    int width = 0;
    int x0 = 0;
    int y0 = 0;
    float lineW = 1.0f;
public:
    void setSize(const int& w, const int& x, const int& y, const float& l) { width = w; x0 = x; y0 = y; lineW = l;}
    void render() override {
        glPushMatrix();
        glTranslatef(position.x(), position.y(), 0);
        glRotatef(rotation, 0, 0, 1);
        glScalef(scale.x(), scale.y(), 1);

        glColor3f(color.x(), color.y(), color.z());

        glLineWidth(lineW);
        glBegin(GL_LINES);
        glVertex2f(x0, y0);
        glVertex2f(x0 + width, y0);
        glEnd();

        glPopMatrix();
    }
};
