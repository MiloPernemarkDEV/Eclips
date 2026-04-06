#include "eclips_renderer.h"
#include <memory>

int main() {
	std::unique_ptr<eclips_renderer> renderer = std::make_unique<eclips_renderer>();	

	try { renderer->run(); }
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}