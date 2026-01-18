#pragma once
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include "Logger.h"
#include <QPointer>

class Camera;
class HUD;
class MyOpenGLWidget;
struct AddActionDesc {
	QString title;
	QString type;
	bool only3D = false;
};
class GUI {
private:
	std::vector<AddActionDesc> addActions = {
	{"Add platform", "platform", true},
	{"Add rectangle", "rectangle"},
	{"Add point", "point"},
	{"Add circle", "circle"},
	{"Add triangle", "triangle"},
	{"Add star", "star"},
	{"Add polygon", "polygon"},
	{"Add line", "line"},
	{"Add cone", "cone", true},
	{"Add cylinder", "cylinder", true},
	{"Add flat", "flat", true}
	};
	QVector<QAction*> only3DActions;
public:
	QPointer<MyOpenGLWidget> oglPtr;
	GUI() {
		LOG_INFO("[GUI] GUI Added");
	}
	~GUI() {
		LOG_INFO("[GUI] GUI Destroyed");
	}
	void updateModeUI(MyOpenGLWidget* ogl);
	void addContexMenu(QMouseEvent* event, MyOpenGLWidget* ogl, QWidget* parentWindow);
	void aboutWindow();
	void fillObjectsMenu(QMenu* parentMenu, MyOpenGLWidget* ogl, bool withExit, QWidget* parentWindow);
	void addMenu(QMainWindow* w, MyOpenGLWidget* ogl);
	void openSceneWindow(QPointer<MyOpenGLWidget> oglPtr);
	void openScenariosWindow(QPointer<MyOpenGLWidget> oglPtr);
	void openRemoveWindow(MyOpenGLWidget* ogl);
	void openChangeWindow(MyOpenGLWidget* ogl);
	void openCameraWindow(Camera& cam);
	void addObjWindow(const std::string& type, MyOpenGLWidget* ogl);
	void openHudWindow(MyOpenGLWidget* ogl,HUD* hud);
	void openTreeWindow(QPointer<MyOpenGLWidget> oglPtr);
};
