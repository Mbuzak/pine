#ifndef SCENE_HPP
#define SCENE_HPP

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <filesystem>
#include "engine/model.hpp"
#include "engine/mesh.hpp"
#include "engine/light.hpp"
#include "engine/pipeline.hpp"
#include "engine/texture.hpp"
#include "engine/shadow.hpp"
#include "engine/camera.hpp"
#include "engine/framebuffer.hpp"
#include "chess/chess.hpp"
#include <GL/freeglut.h>

class Scene {
public:
	Scene();

	void Setup();
	void Display();

	void SendLight();

//private:
	GLuint sbp;
	GLuint program_default;

	Skybox *skybox_ = nullptr;
	Camera camera_;

	std::map<std::string, Model*> models_;
	std::map<std::string, Texture*> textures_;

	// --- meshes ---
	Sun *sun_ = nullptr;
	std::array<Lamp*, 4> lamps_;

	std::vector<Shape*> background_;

	ShadowMap dir_shadow_map;
	Framebuffer fbo;

private:
	void RenderShadowMapOfDirectionalLight();

	void RenderToTexture();

	void RenderSkybox();
	void RenderShapes();
	void RenderLights();

public:
	std::vector<Piece*> get_pieces();

	void Init();
	void Display(GLuint program_id);

	void UpdatePieceWorldPosition(int id, float x, float z);

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
	int _mouse_buttonState = GLUT_UP;
	int _mouse_left_click_state = GLUT_UP;
	int _mouse_buttonX;
	int _mouse_buttonY;

	void select_piece(int wx, int wy, int x, int y);
	void move_piece();

	// Rotate camera
	void rotate(int, int);

	int width = 800, height = 600;
};

#endif
