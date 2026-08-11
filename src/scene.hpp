#ifndef PINE_SCENE
#define PINE_SCENE

#include <stdlib.h>
#include <string>
#include <array>
#include "SDL2/SDL.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "engine/shadow.h"
#include "engine/outline.h"
#include "engine/terrain.h"
#include "engine/rendered.h"
#include "engine/entity.h"
#include "engine/mesh.h"
#include "engine/camera.h"
#include "engine/shader.h"
#include "engine/display.h"
#include "engine/controller.h"
#include "engine/texture.h"
#include "engine/light.h"
#ifdef __cplusplus
}
#endif

#define __CHECK_FOR_ERRORS 	{GLenum errCode; if ((errCode = glGetError()) != GL_NO_ERROR) printf("Error (%d): %s in file %s at line %d !\n", errCode, gluErrorString(errCode), __FILE__,  __LINE__);}

enum Shaders {
	SHADERS_RENDERED,
	SHADERS_OUTLINE,
	SHADERS_SHADOW_MAP,
	SHADERS_TERRAIN,
	SHADERS_COUNT
};

typedef struct {
	GLuint id;
	GLuint* blocks_id;
} UBO;

void ubo_init(UBO* ubo, int binding_id, int shader_count, GLuint* shaders_id, const char* block_name, int size);
void ubo_uniform_mat4_send(GLuint ubo_id, int offset, mat4s matrix);

class Scene {
public:
	Scene() = default;

	Display d;
	RendererTerrain renderer_terrain;

	Shader shaders[SHADERS_COUNT];

	Camera camera;
	Camera camera_light;
	Controller controller;

	Mesh mesh_cube;

	Sun sun;
	std::array<Lamp, 4> lamps;
	const int shape_count = 32;
	Shape shapes[32];

	ShadowMap dir_shadow_map;

	int selected_id = -1;

	UBO ubo;
	UBO ubo_light;
private:
	int events_handle();
	void rotate(int, int);

public:
	void Setup();
	void display();
};

mat4s orthographic_projection_compute();

#endif
