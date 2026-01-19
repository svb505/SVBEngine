#pragma once
#include "GUI.h"
#include "Logger.h"

class OpenGLW;
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
	HUD(OpenGLW* ogl, Camera* camera, GUI* gui) {
		LOG_INFO("[HUD] HUD Created");
		drawHud(ogl, camera, gui);
	}
	~HUD() {
		LOG_INFO("[HUD] HUD Destroyed");
	}
	void drawHud(OpenGLW* ogl,Camera* camera,GUI* gui);
};