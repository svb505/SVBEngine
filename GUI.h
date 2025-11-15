#pragma once

class MyOpenGLWidget;
class GUI {
public:
	void addMenu(QMainWindow* w, MyOpenGLWidget* ogl);
	void openPhysicsWindow();
	void openSceneWindow();
	void openScenariosWindow();
	void addObjWindow(const std::string& type,MyOpenGLWidget* ogl);
	void changeMode(const std::string& mode,MyOpenGLWidget* ogl);
};
