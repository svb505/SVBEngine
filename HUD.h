#pragma once

class MyOpenGLWidget;
class Camera;
class HUD {
private:
	int startX = -390;
	int startY = 190;
	int step = 10;
	int idx = 0;
public:
	void drawHud(MyOpenGLWidget* ogl,Camera* camera);
};