#include <time.h>
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
	srand(time(NULL));
	Scene app;
	app.Setup();
	app.display();

	return 0;
}
