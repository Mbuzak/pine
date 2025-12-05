#include "scene.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, char *argv[]) {
	Scene scene;
	scene.Setup();
	scene.display();

	return 0;
}
