#ifndef PINE_RENDERER
#define PINE_RENDERER

#include <GL/glew.h>
#include "mesh.hpp"
#include "camera.hpp"
#include "entity.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "shader.h"
#include "texture.h"
#ifdef __cplusplus
}
#endif

typedef struct {
	GLuint program_id;
	Mesh mesh;
	GLuint texture_id;
} RendererSkybox;

struct RendererStatic {
	ShaderStatic shader;
	float fov;
	float plane_near;
	float plane_far;
};

void renderer_static_init(RendererStatic*);

int renderer_skybox_init(RendererSkybox*);
void renderer_skybox_destroy(RendererSkybox*);
void renderer_skybox_render(RendererSkybox*, Camera* camera);

// Render colored solid model
void solid_render(GLuint, Transform*, Mesh*);

#endif
