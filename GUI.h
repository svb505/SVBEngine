#pragma once
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include "Logger.h"
#include <QPointer>

class Camera;
class HUD;
class MyOpenGLWidget;
class GUI {
public:
	QPointer<MyOpenGLWidget> oglPtr;
	GUI() {
		LOG_INFO("[GUI] GUI Added");
	}
	~GUI() {
		LOG_INFO("[GUI] GUI Destroyed");
	}
	void addContexMenu(QMouseEvent* event, MyOpenGLWidget* ogl, QWidget* parentWindow);
	void aboutWindow();
	void addMenu(QMainWindow* w, MyOpenGLWidget* ogl);
	void openSceneWindow(MyOpenGLWidget* ogl);
	void openScenariosWindow(QPointer<MyOpenGLWidget> oglPtr);
	void openRemoveWindow(MyOpenGLWidget* ogl);
	void openChangeWindow(MyOpenGLWidget* ogl);
	void openCameraWindow(Camera& cam);
	void addObjWindow(const std::string& type, MyOpenGLWidget* ogl);
	void openHudWindow(MyOpenGLWidget* ogl,HUD* hud);
	void openTreeWindow(QPointer<MyOpenGLWidget> oglPtr);
};
