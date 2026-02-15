#pragma once
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>   
#include <string>

void drawGridOpenGL(float spacing, int count,std::string mode) {
    glPushMatrix();
    glLineWidth(1.0f);
    glBegin(GL_LINES);

    if (mode == "2D") {
        for (int i = -count; i <= count; ++i) {
            if (i == 0) glColor3f(1.0f, 0.0f, 0.0f);
            else glColor3f(0.5f, 0.5f, 0.5f);

            float pos = i * spacing;
            glVertex3f(pos, -count * spacing, 0);
            glVertex3f(pos, count * spacing, 0);
            glVertex3f(-count * spacing, pos, 0);
            glVertex3f(count * spacing, pos, 0);
        }
    }
    else if (mode == "3D") {
        for (int i = -count; i <= count; ++i) {
            float pos = i * spacing;
            glColor3f(1.0f, 1.0f, 1.0f);
            glVertex3f(-count * spacing, 0, pos);
            glVertex3f(count * spacing, 0, pos);

            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(pos, 0, -count * spacing);
            glVertex3f(pos, 0, count * spacing);
        }
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0, -count * spacing, 0);
        glVertex3f(0, count * spacing, 0);
    }

    glEnd();
    glPopMatrix();
}