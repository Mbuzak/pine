#ifndef PINE_SCENE
#define PINE_SCENE

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include "engine/mesh.hpp"
#include "engine/entity.hpp"
#include "engine/light.hpp"
#include "engine/shader.hpp"
#include "engine/texture.hpp"
#include "engine/shadow.hpp"
#include "engine/camera.hpp"
#include "engine/framebuffer.hpp"
#include "engine/renderer.hpp"
#include "engine/uniform.hpp"
#include "chess/chess.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "engine/display.h"
#ifdef __cplusplus
}
#endif

#define __CHECK_FOR_ERRORS 	{GLenum errCode; if ((errCode = glGetError()) != GL_NO_ERROR) printf("Error (%d): %s in file %s at line %d !\n", errCode, gluErrorString(errCode), __FILE__,  __LINE__);}

class Scene {
public:
	Scene();

	void Setup();
	void display();

	Display d;
	RendererSkybox renderer_skybox;

//private:
	GLuint program_default;
	GLuint program_color;

	Camera camera;

	std::map<std::string, Mesh*> meshes;
	std::map<std::string, GLuint> textures;

	// --- meshes ---
	Sun *sun_ = nullptr;
	std::array<Lamp*, 4> lamps_;

	std::vector<Shape*> background_;

	ShadowMap dir_shadow_map;
	Framebuffer fbo;

private:
	void RenderToTexture();
	void RenderShapes(GLuint);
	void RenderLights();

public:
	std::vector<Piece*> get_pieces();

	void DisactivatePiece(Piece &piece);

	chschr::Chess* chess;
	std::vector<int> active_fields;

	glm::vec3 IndexToPosition(int id);

	std::vector<Piece*> pieces_;
	std::array<Shape*, 64> squares_;
	float off_rank_white = -8.0;
	float off_rank_black = -8.0;
	int selected_id = -1;

	// Zmienne do kontroli stanu myszy
	int _mouse_buttonState = 0;
	int _mouse_left_click_state = 0;
	int _mouse_buttonX;
	int _mouse_buttonY;

	void select_piece(int wx, int wy, int x, int y);
	void move_piece();
	void motion(int x, int y);
	void reshape(int, int);

	// Rotate camera
	void rotate(int, int);

	int width = 800, height = 600;
};

#endif
