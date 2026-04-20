#ifndef ECLIPS_APPLICATION_H
#define ECLIPS_APPLICATION_H

#include <EclipsInstance.h>
#include <EclipsWindow.h>
#include <EclipsRenderer.h>
#include "Camera.h"	

class Application {
public:
	Application();
	~Application();
	bool init();
	void run();
	void end();
private:
	EclipsInstance eclipsInstance;
	EclipsWindow eclipsWindow;
	EclipsRenderer eclipsRenderer;
	Camera camera;
};

#endif // ECLIPS_APPLICATION_H