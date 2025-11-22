#pragma once
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>

class MyOpenGLWidget;
class GUI {
public:
	void addMenu(QMainWindow* w, MyOpenGLWidget* ogl);
	void openSceneWindow(MyOpenGLWidget* ogl);
	void openScenariosWindow(MyOpenGLWidget* ogl);
	void openRemoveWindow(MyOpenGLWidget* ogl);
	void addObject(std::string& type, const  std::string& name, MyOpenGLWidget* ogl, const float& x,
		const float& y, const float& z,float colors[], std::map<std::string, float>& positions);
	void addObjWindow(const std::string& type,MyOpenGLWidget* ogl);
	void changeMode(const std::string& mode,MyOpenGLWidget* ogl);
	void movingParcer(MyOpenGLWidget* ogl, const std::string& name, const int& repeatTime,const QStringList& items,
		const std::map<std::string,std::string>& scenarios,const int& speed);
	void startMoveObj(MyOpenGLWidget* ogl, const std::string& name,const int& speed, const std::string& vect,
		const int time, const int to, std::function<void()> onFinished);
};
