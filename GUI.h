#pragma once

class MyOpenGLWidget;
class GUI {
public:
	void addMenu(QMainWindow* w, MyOpenGLWidget* ogl);
	void openPhysicsWindow();
	void openSceneWindow();
	void openScenariosWindow();
	void openRemoveWindow(MyOpenGLWidget* ogl);
	void addObject(std::string& type, const  std::string& name, MyOpenGLWidget* ogl, const float& x,
		const float& y, const float& z,
		float colors[], std::map<std::string, float>& positions);
	void addObjWindow(const std::string& type,MyOpenGLWidget* ogl);
	void changeMode(const std::string& mode,MyOpenGLWidget* ogl);
};
