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
#include "engine/texture.hpp"
#include "engine/shadow.hpp"
#include "engine/camera.hpp"
#include "engine/renderer.hpp"
#include "engine/uniform.hpp"
#include "chess/chess.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "engine/shader.h"
#include "engine/display.h"
#include "engine/fbo.h"
#include "engine/controller.h"
#ifdef __cplusplus
}
#endif

#define __CHECK_FOR_ERRORS 	{GLenum errCode; if ((errCode = glGetError()) != GL_NO_ERROR) printf("Error (%d): %s in file %s at line %d !\n", errCode, gluErrorString(errCode), __FILE__,  __LINE__);}

class Scene {
public:
	Scene();


	Display d;
	RendererSkybox renderer_skybox;

	GLuint program_default;
	GLuint program_color;

	Camera camera;
	Controller controller;

	std::map<std::string, Mesh*> meshes;
	std::map<std::string, GLuint> textures;

	Sun sun;
	std::array<Lamp*, 4> lamps_;
	Shape terrain;
	Shape board;
	std::vector<Piece*> pieces_;
	std::array<Shape*, 64> squares_;

	ShadowMap dir_shadow_map;
	Framebuffer fbo;

	chschr::Chess* chess;
	std::vector<int> active_fields;

	float off_rank_white = -8.0;
	float off_rank_black = -8.0;
	int selected_id = -1;

private:
	void RenderToTexture();
	void RenderShapes(GLuint);
	void RenderLights();

	int events_handle();
	void select_piece(int wx, int wy, int x, int y);
	void move_piece();
	void motion(int x, int y);
	void reshape(int, int);
	void rotate(int, int);

public:
	void Setup();
	void display();

	std::vector<Piece*> get_pieces();
	glm::vec3 IndexToPosition(int id);
	void DisactivatePiece(Piece &piece);
};

#endif
