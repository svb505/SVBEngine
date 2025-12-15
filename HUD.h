#pragma once
#include "GUI.h"

class MyOpenGLWidget;
class Camera;
class GUI;
class HUD {
	friend class GUI;
private:
	int startX = -390;
	int startY = 190;
	int step = 10;
	int idx = 0;
	int count = 8;
	std::vector<bool> hudTextsVisible = {true,true,true,true,true,true,true,true};
	std::vector<std::string> hudTexts = {"FPS","Objects count","Mode","Camera incline","Camera horizontal",
		"Camera X offcet","Camera Y offcet","Camera distance from center"};
public:
	void drawHud(MyOpenGLWidget* ogl,Camera* camera,GUI* gui);
};