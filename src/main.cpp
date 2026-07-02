#include "scene.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/* TODO
 * Rename Scene to App
 * Change Scene methods to functions
 */
int main(int argc, char *argv[]) {
	Scene app;
	app_init(&app, 1920, 1080, "pine");

	app.camera.pos = {0.0, -3.0, -22.0};
	app.camera.rot = {15, 270};

	app.program_default = program_init("default");
	app.program_color = program_init("color");

	// Load models
	std::vector<std::string> model_names = {"square", "pawn", "knight", "bishop", "rook", "king", "queen", "chessboard", "sphere"};
	for (std::string &name: model_names) {
		Mesh mesh;
		mesh_texture_init(&mesh, name);
		app.meshes.insert({name, mesh});
	}

	// Load textures
	app.textures = new GLuint[3];
	app.textures[TEX_WHT] = texture_2d_init("white.jpg");
	app.textures[TEX_BLC] = texture_2d_init("black.jpg");
	app.textures[TEX_CHS] = texture_2d_init("chessboard.jpg");

	app.Setup();

	app.display();

	return 0;
}
