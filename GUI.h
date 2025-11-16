#pragma once
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>

class MyOpenGLWidget;
class GUI {
public:
	QLabel* makeLabel(QWidget* parent, const QString& text, const int& x, const int& y);
	QLineEdit* makeLineEdit(QWidget* parent, const QString& text, const QString& setTxt, const int& x, const int& y);
	void addMenu(QMainWindow* w, MyOpenGLWidget* ogl);
	void openPhysicsWindow(MyOpenGLWidget* ogl);
	void openSceneWindow(MyOpenGLWidget* ogl);
	void openScenariosWindow(MyOpenGLWidget* ogl);
	void openRemoveWindow(MyOpenGLWidget* ogl);
	void addObject(std::string& type, const  std::string& name, MyOpenGLWidget* ogl, const float& x,
		const float& y, const float& z,float colors[], std::map<std::string, float>& positions);
	void addObjWindow(const std::string& type,MyOpenGLWidget* ogl);
	void changeMode(const std::string& mode,MyOpenGLWidget* ogl);
	void movingParcer(MyOpenGLWidget* ogl, const std::string& name, const int& repeatTime,const QStringList& items,
		const std::map<std::string,std::string>& scenarios);
	void startMoveObj(MyOpenGLWidget* ogl, const std::string& name, const std::string& vect,
		const int time, const int to, std::function<void()> onFinished);
};
