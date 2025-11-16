#pragma once
#include <QStringList>
#include <map>
#include <string>
#include <functional>
class MyOpenGLWidget;

class Phys {
private:
	float objVelocity = 0.99f;
public:
	void movingParcer(MyOpenGLWidget* ogl, const std::string& name, const int& repeatTime,
		const QStringList& items, const std::map<std::string, std::string>& scenarios);
	void startMoveObj(MyOpenGLWidget* ogl, const std::string& name, const std::string& vect,
		const int time, const int to, std::function<void()> onFinished);
};
