#pragma once
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>

class HUD;
class MyOpenGLWidget;
class GUI {
public:
	void addContexMenu(QMouseEvent* event, MyOpenGLWidget* ogl, QWidget* parentWindow);
	void aboutWindow();
	void addMenu(QMainWindow* w, MyOpenGLWidget* ogl);
	void openSceneWindow(MyOpenGLWidget* ogl);
	void openScenariosWindow(MyOpenGLWidget* ogl);
	void openRemoveWindow(MyOpenGLWidget* ogl);
	void openChangeWindow(MyOpenGLWidget* ogl);
	void addObjWindow(const std::string& type, MyOpenGLWidget* ogl);
	void openHudWindow(MyOpenGLWidget* ogl,HUD* hud);
};
