#pragma once
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>

class MyOpenGLWidget;
class GUI {
public:
	void aboutWindow();
	void addMenu(QMainWindow* w, MyOpenGLWidget* ogl);
	void openSceneWindow(MyOpenGLWidget* ogl);
	void openScenariosWindow(MyOpenGLWidget* ogl);
	void openRemoveWindow(MyOpenGLWidget* ogl);
	void openChangeWindow(MyOpenGLWidget* ogl);
	void addObjWindow(const std::string& type,MyOpenGLWidget* ogl);
};
