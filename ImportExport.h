#pragma once
#include <iostream>
#include "MyOpenGLWidget.h"

class MyOpenGLWidget;

class ImpExp {
public:
	std::map<std::string, Data> importScene(const QString& fileName, MyOpenGLWidget* ogl);
	void exportScene(const QString& fileName, const std::map<std::string, Data>& objects, MyOpenGLWidget* ogl);
	void exportSceneWithDialog(const std::map<std::string, Data>& objects, MyOpenGLWidget* ogl);
	std::map<std::string, Data> importSceneWithDialog(MyOpenGLWidget* ogl);
	void exportScenarios(QWidget* child, QListWidget* list1);
	void importScenarios(QWidget* child, QListWidget* list1);
};
	