#include "Application.h"
#include <memory>

int main() {
	std::unique_ptr<Application> app = std::make_unique<Application>();

	if (!app->init()) {
		return 1;
	}

	app->run();

	app->end();

	return EXIT_SUCCESS;
}