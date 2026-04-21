#pragma once

#include <Instance.h>
#include <Platform.h>
#include <Renderer.h>
#include "Camera.h"	

class Application {
public:
	Application();
	~Application();
	bool init();
	void run();
	void end();
private:
	Instance instance;
	Platform platform;
	Renderer renderer;
	Camera camera;
};