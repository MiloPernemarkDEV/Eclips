#include "eclips_renderer.h"

int main() {
	eclips_renderer test_renderer;

	try { test_renderer.run(); }
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}