#ifndef PINE_SCENE
#define PINE_SCENE

#define TEX_WHT 0
#define TEX_BLC 1

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include "engine/mesh.hpp"
#include "engine/entity.hpp"
#include "engine/shadow.hpp"
#include "engine/skybox.hpp"
#include "engine/rendered.hpp"
#include "engine/outline.hpp"
#include "engine/terrain.hpp"
#include "SDL2/SDL.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "engine/camera.h"
#include "engine/shader.h"
#include "engine/display.h"
#include "engine/frame.h"
#include "engine/controller.h"
#include "engine/texture.h"
#include "engine/light.h"
#ifdef __cplusplus
}
#endif

#define __CHECK_FOR_ERRORS 	{GLenum errCode; if ((errCode = glGetError()) != GL_NO_ERROR) printf("Error (%d): %s in file %s at line %d !\n", errCode, gluErrorString(errCode), __FILE__,  __LINE__);}

enum Shaders {
	SHADERS_RENDERED,
	SHADERS_SKYBOX,
	SHADERS_OUTLINE,
	SHADERS_SHADOW_MAP,
	SHADERS_COUNT
};

class Scene {
public:
	Scene() = default;

	Display d;
	RendererSkybox renderer_skybox;
	RendererTerrain renderer_terrain;

	Shader shaders[SHADERS_COUNT];

	Camera camera;
	Camera camera_light;
	Controller controller;

	std::map<std::string, Mesh> meshes;
	GLuint* textures;

	Sun sun;
	std::array<Lamp, 4> lamps;
	std::vector<Shape> pieces;

	ShadowMap dir_shadow_map;

	int selected_id = -1;

private:
	void RenderToTexture(GLuint);
	void RenderShapes(GLuint);

	int events_handle();
	void motion(int, int);
	void rotate(int, int);

public:
	void Setup();
	void display();
};

mat4s perspective_projection_compute(float, float);
mat4s orthographic_projection_compute();

int selection_id_compute(int, int);

#endif
