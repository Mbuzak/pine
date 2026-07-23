#include "scene.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/* TODO
 * Rename Scene to App
 * Change Scene methods to functions
 */
/* FIXME
 * Keys out of 128 ASCII brakes program
 */
int main(int argc, char *argv[]) {
	Scene app;
	app_init(&app, 1280, 720, "pine");
	app.Setup();
	app.display();

	return 0;
}
