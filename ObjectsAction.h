#pragma once
#include <iostream>

class MyOpenGLWidget;
class Action {
private:
	struct Scenario {
		std::string vect;
		int num;
	};
public:
	void movingParcer(MyOpenGLWidget* ogl, const std::string& name, const int& repeatTime, const QStringList& items,
		const std::map<std::string, std::string>& scenarios, const int& speed);
	void startMoveObj(MyOpenGLWidget* ogl, const std::string& name, const int& speed, const std::string& vect,
		const int time, const int to, std::function<void()> onFinished);
	void addObject(std::string& type, const  std::string& name, MyOpenGLWidget* ogl, const float& x,
		const float& y, const float& z, float colors[], std::map<std::string, float>& positions, bool& dMode);
};
