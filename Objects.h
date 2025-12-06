#pragma once
#include <QOpenGLFunctions>
#include <QVector3D>
#include <cmath>

class Object : protected QOpenGLFunctions {
public:
    float rotation = 0.0f;
    int turnX = 0;
    int turnZ = 0;
    bool mode = false;
    QVector3D position{ 0, 0, 0 };
    QVector3D scale{ 1, 1, 1 };
    QVector3D color{ 1, 1, 1 };    

    Object() { initializeOpenGLFunctions(); }
    virtual void render() = 0;  
    float x() const { return position.x(); }
    float y() const { return position.y(); }
    float z() const { return position.z(); }
    bool getMode(){ return mode; }
    virtual ~Object() {}
};
class Box : public Object {
private:
    float width = 1.0f;
    float height = 1.0f;
public:
    float getW() { return width; }
    float getH() { return height; }
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
class Point : public Object {
private:
    float pointSize = 5.0f; 
public:
    float getSize() { return pointSize; }
    void setSize(const float& s) { pointSize = s; }

    void render() override {
        glPushMatrix();
        glTranslatef(position.x(), position.y(), position.z());

        glColor3f(color.x(), color.y(), color.z());

        glEnable(GL_POINT_SMOOTH); 
        glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

        glPointSize(pointSize);

        glBegin(GL_POINTS);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glEnd();

        glPopMatrix();
    }

};
class Triangle : public Object {
private:
    float base = 1.0f;
    float height = 1.0f;
public:
    float getH() { return height; }
    float getBase() { return base; }
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
    int getSides() { return sides; }
    float getRadius() { return radius; }
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

    int getPoints() { return points; }
    float getOuter() { return outerRadius; }
    float getInner(){ return innerRadius; }
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
    int getWidth() { return width;}
    int getX0(){ return x0;}
    int getY0(){ return y0; }
    float getLineW() { return lineW; }
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
class Cube : public Object {
public:
    float width = 1.0f;
    float height = 1.0f;
    float depth = 1.0f;

    void setSize(float w, float h, float d) {
        width = w;
        height = h;
        depth = d;
    }
    void render() override {
        glPushMatrix();

        glTranslatef(position.x(), position.y(), position.z());
        glRotatef(rotation, turnX, 1, turnZ);
        glScalef(scale.x(), scale.y(), scale.z());
        glColor3f(color.x(), color.y(), color.z());

        float w = width / 2.0f;
        float h = height / 2.0f;
        float d = depth / 2.0f;

        if (mode) glBegin(GL_LINE_STRIP);
        else glBegin(GL_QUADS);
        
        // Front
        glNormal3f(0, 0, 1);
        glVertex3f(-w, -h, d);
        glVertex3f(w, -h, d);
        glVertex3f(w, h, d);
        glVertex3f(-w, h, d);

        // Back
        glNormal3f(0, 0, -1);
        glVertex3f(-w, -h, -d);
        glVertex3f(-w, h, -d);
        glVertex3f(w, h, -d);
        glVertex3f(w, -h, -d);

        // Left
        glNormal3f(-1, 0, 0);
        glVertex3f(-w, -h, -d);
        glVertex3f(-w, -h, d);
        glVertex3f(-w, h, d);
        glVertex3f(-w, h, -d);

        // Right
        glNormal3f(1, 0, 0);
        glVertex3f(w, -h, -d);
        glVertex3f(w, h, -d);
        glVertex3f(w, h, d);
        glVertex3f(w, -h, d);

        // Top
        glNormal3f(0, 1, 0);
        glVertex3f(-w, h, -d);
        glVertex3f(-w, h, d);
        glVertex3f(w, h, d);
        glVertex3f(w, h, -d);

        // Bottom
        glNormal3f(0, -1, 0);
        glVertex3f(-w, -h, -d);
        glVertex3f(w, -h, -d);
        glVertex3f(w, -h, d);
        glVertex3f(-w, -h, d);

        glEnd();

        glPopMatrix();
    }
};
class Sphere : public Object {
private:
    float radius = 1.0f;
    int slices = 24;
    int stacks = 24;
public:
    int getSlices() { return slices;  }
    int getStacks(){ return stacks; }
    float getRadius() { return radius; }
    
    void setSize(float r, int sl = 24, int st = 24) {
        radius = r;
        slices = sl;
        stacks = st;
    }
    void render() override {
        glPushMatrix();

        glTranslatef(position.x(), position.y(), position.z());
        glRotatef(rotation, turnX, 1, turnZ);
        glScalef(scale.x(), scale.y(), scale.z());
        glColor3f(color.x(), color.y(), color.z());

        for (int i = 0; i < stacks; ++i) {
            float lat0 = M_PI * (-0.5 + (double)i / stacks);
            float z0 = radius * sin(lat0);
            float zr0 = radius * cos(lat0);

            float lat1 = M_PI * (-0.5 + (double)(i + 1) / stacks);
            float z1 = radius * sin(lat1);
            float zr1 = radius * cos(lat1);
            if (!mode) glBegin(GL_QUAD_STRIP);
            else glBegin(GL_LINE_STRIP);
            for (int j = 0; j <= slices; ++j) {
                float lng = 2 * M_PI * (double)(j) / slices;
                float x = cos(lng);
                float y = sin(lng);

                glNormal3f(x * zr0, y * zr0, z0);
                glVertex3f(x * zr0, y * zr0, z0);

                glNormal3f(x * zr1, y * zr1, z1);
                glVertex3f(x * zr1, y * zr1, z1);
            }
            glEnd();
        }

        glPopMatrix();
    }
};
class Pyramid : public Object {
public:
    float base = 1.0f;
    float height = 1.0f;
    void setSize(float b, float h) {
        base = b;
        height = h;
    }
    void render() override {
        glPushMatrix();

        glTranslatef(position.x(), position.y(), position.z());
        glRotatef(rotation, turnX, 1, turnZ);
        glScalef(scale.x(), scale.y(), scale.z());
        glColor3f(color.x(), color.y(), color.z());

        float b = base / 2.0f;

        if (!mode) glBegin(GL_TRIANGLES); 
        else glBegin(GL_LINE_STRIP);
        
        // Front
        glNormal3f(0, 0.5, 1);
        glVertex3f(0, height, 0);
        glVertex3f(-b, 0, b);
        glVertex3f(b, 0, b);

        // Right
        glNormal3f(1, 0.5, 0);
        glVertex3f(0, height, 0);
        glVertex3f(b, 0, b);
        glVertex3f(b, 0, -b);

        // Back
        glNormal3f(0, 0.5, -1);
        glVertex3f(0, height, 0);
        glVertex3f(b, 0, -b);
        glVertex3f(-b, 0, -b);

        // Left
        glNormal3f(-1, 0.5, 0);
        glVertex3f(0, height, 0);
        glVertex3f(-b, 0, -b);
        glVertex3f(-b, 0, b);

        glEnd();

        // Bottom
        if (!mode) glBegin(GL_QUADS);
        else glBegin(GL_LINE_STRIP);
        glNormal3f(0, -1, 0);
        glVertex3f(-b, 0, -b);
        glVertex3f(b, 0, -b);
        glVertex3f(b, 0, b);
        glVertex3f(-b, 0, b);
        glEnd();

        glPopMatrix();
    }
};
class Prism : public Object {
private:
    int sides = 6;
    float radius = 1.0f;
    float height = 1.0f;
public:
    float getHeight(){ return height; }
    float getRadius() { return radius;}
    int getSides() { return sides; }

    void setSize(int s, float r, float h) {
        sides = s;
        radius = r;
        height = h;
    }
    void render() override {
        glPushMatrix();

        glTranslatef(position.x(), position.y(), position.z());
        glRotatef(rotation, turnX, 1, turnZ);
        glScalef(scale.x(), scale.y(), scale.z());
        glColor3f(color.x(), color.y(), color.z());

        float halfH = height / 2.0f;

        // SIDES
        if (mode) glBegin(GL_LINE_STRIP);
        else glBegin(GL_QUADS);
        for (int i = 0; i < sides; i++) {
            float a1 = 2 * M_PI * i / sides;
            float a2 = 2 * M_PI * (i + 1) / sides;

            float x1 = cos(a1) * radius;
            float y1 = sin(a1) * radius;

            float x2 = cos(a2) * radius;
            float y2 = sin(a2) * radius;

            glNormal3f(x1, y1, 0);
            glVertex3f(x1, halfH, y1);
            glVertex3f(x2, halfH, y2);
            glVertex3f(x2, -halfH, y2);
            glVertex3f(x1, -halfH, y1);
        }
        glEnd();

        // TOP
        if (mode) glBegin(GL_LINE_STRIP);
        else glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0, 1, 0);
        glVertex3f(0, halfH, 0);
        for (int i = 0; i <= sides; i++) {
            float a = 2 * M_PI * i / sides;
            glVertex3f(cos(a) * radius, halfH, sin(a) * radius);
        }
        glEnd();

        // BOTTOM
        if (mode) glBegin(GL_LINE_STRIP);
        else glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0, -1, 0);
        glVertex3f(0, -halfH, 0);
        for (int i = 0; i <= sides; i++) {
            float a = 2 * M_PI * i / sides;
            glVertex3f(cos(a) * radius, -halfH, sin(a) * radius);
        }
        glEnd();

        glPopMatrix();
    }
};
class Cylinder : public Object {
private:
    float radiusTop = 0.5f;
    float radiusBottom = 0.5f;
    float height = 1.0f;
    int segments = 36; 
public:
    float getRT() { return radiusTop; }
    float getRB() { return radiusBottom; }
    float getH() { return height; }
    void setSize(float rTop, float rBottom, float h) {
        radiusTop = rTop;
        radiusBottom = rBottom;
        height = h;
    }
    void setSegments(int seg) { segments = seg; }
    void render() override {
        glPushMatrix();
        glTranslatef(position.x(), position.y(), position.z());
        glRotatef(rotation, turnX, 1, turnZ);
        glScalef(scale.x(), scale.y(), scale.z());
        glColor3f(color.x(), color.y(), color.z());

        if (!mode) glBegin(GL_TRIANGLE_STRIP);
        else glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= segments; ++i) {
            float angle = i * 2.0f * M_PI / segments;
            float xTop = radiusTop * cos(angle);
            float zTop = radiusTop * sin(angle);
            float xBottom = radiusBottom * cos(angle);
            float zBottom = radiusBottom * sin(angle);

            glVertex3f(xTop, height / 2, zTop);
            glVertex3f(xBottom, -height / 2, zBottom);
        }
        glEnd();

        if (!mode) glBegin(GL_TRIANGLE_FAN);
        else  glBegin(GL_LINE_STRIP);
        glVertex3f(0, height / 2, 0); 
        for (int i = 0; i <= segments; ++i) {
            float angle = i * 2.0f * M_PI / segments;
            glVertex3f(radiusTop * cos(angle), height / 2, radiusTop * sin(angle));
        }
        glEnd();

        if (!mode) glBegin(GL_TRIANGLE_FAN);
        else  glBegin(GL_LINE_STRIP);
        glVertex3f(0, -height / 2, 0); 
        for (int i = 0; i <= segments; ++i) {
            float angle = i * 2.0f * M_PI / segments;
            glVertex3f(radiusBottom * cos(angle), -height / 2, radiusBottom * sin(angle));
        }
        glEnd();

        glPopMatrix();
    }
};
class Cone : public Object {
private:
    float radius = 0.5f;
    float height = 1.0f;
    int segments = 36;
public:
    void setSize(float r, float h) {
        radius = r;
        height = h;
    }
    float getR() { return radius; }
    float getH() { return height; }
    void setSegments(int seg) { segments = seg; }
    void render() override {
        glPushMatrix();
        glTranslatef(position.x(), position.y(), position.z());
        glRotatef(rotation, turnX, 1, turnZ);
        glScalef(scale.x(), scale.y(), scale.z());
        glColor3f(color.x(), color.y(), color.z());

        if (!mode) glBegin(GL_TRIANGLES);
        else glBegin(GL_LINE_STRIP);
        for (int i = 0; i < segments; ++i) {
            float angle1 = i * 2.0f * M_PI / segments;
            float angle2 = (i + 1) * 2.0f * M_PI / segments;
            glVertex3f(0, height / 2, 0);
            glVertex3f(radius * cos(angle1), -height / 2, radius * sin(angle1));
            glVertex3f(radius * cos(angle2), -height / 2, radius * sin(angle2));
        }
        glEnd();

        if (!mode) glBegin(GL_TRIANGLE_FAN);
        else glBegin(GL_LINE_STRIP);
        glVertex3f(0, -height / 2, 0); 
        for (int i = 0; i <= segments; ++i) {
            float angle = i * 2.0f * M_PI / segments;
            glVertex3f(radius * cos(angle), -height / 2, radius * sin(angle));
        }
        glEnd();

        glPopMatrix();
    }
};