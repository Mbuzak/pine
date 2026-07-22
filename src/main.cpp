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

	app.camera.pos = {-22.0, 9.0, -0.0};
	app.camera.rot = {-15, 0};

	app.program_default = program_init("default");
	app.program_color = program_init("color");
	shader_static_init(&app.shader_static, app.program_default);

	// Load models
	std::vector<std::string> model_names = {"square", "pawn", "knight", "bishop", "rook", "king", "queen", "sphere"};
	for (std::string &name: model_names) {
		Mesh mesh;
		mesh_texture_init(&mesh, name);
		app.meshes.insert({name, mesh});
	}

	// Load textures
	app.textures = new GLuint[2];
	app.textures[TEX_WHT] = texture_2d_init("white.jpg");
	app.textures[TEX_BLC] = texture_2d_init("black.jpg");

	app.Setup();

	app.display();

	return 0;
}
