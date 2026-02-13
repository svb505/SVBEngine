#pragma once
#include <iostream>
#include "OpenGLW.h"

class OpenGLW;

class ImpExp {
public:
	std::map<std::string, Data> importScene(const QString& fileName, OpenGLW* ogl);
	void exportScene(const QString& fileName, const std::unordered_map<std::string, Data>& objects, OpenGLW* ogl);
	void exportSceneWithDialog(const std::unordered_map<std::string, Data>& objects, OpenGLW* ogl);
	std::map<std::string, Data> importSceneWithDialog(OpenGLW* ogl);
	void exportScenarios(QWidget* child, QListWidget* list1);
	void importScenarios(QWidget* child, QListWidget* list1);
	bool exportToJson(Sound& sound, const QString& filename);
	bool importFromJson(Sound& sound, const QString& filename);
};
	