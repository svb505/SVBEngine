#pragma once
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include "Logger.h"
#include <QPointer>
#include "sounds.h"

class Camera;
class HUD;
class OpenGLW;
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
	QPointer<OpenGLW> oglPtr;
	GUI() {
		LOG_INFO("[GUI] GUI Added");
	}
	~GUI() {
		LOG_INFO("[GUI] GUI Destroyed");
	}
	void updateModeUI(OpenGLW* ogl);
	void addContexMenu(QMouseEvent* event, OpenGLW* ogl, QWidget* parentWindow);
	void aboutWindow();
	void soundWindow(Sound& sound);
	void fillObjectsMenu(QMenu* parentMenu, OpenGLW* ogl, bool withExit, QWidget* parentWindow);
	void addMenu(QMainWindow* w, OpenGLW* ogl,Sound& sound);
	void openSceneWindow(QPointer<OpenGLW> oglPtr);
	void openScenariosWindow(QPointer<OpenGLW> oglPtr);
	void openRemoveWindow(OpenGLW* ogl);
	void openChangeWindow(OpenGLW* ogl);
	void openCameraWindow(Camera& cam);
	void addObjWindow(const std::string& type, OpenGLW* ogl);
	void openHudWindow(OpenGLW* ogl,HUD* hud);
	void openTreeWindow(QPointer<OpenGLW> oglPtr);
};
