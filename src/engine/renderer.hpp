#ifndef PINE_RENDERER
#define PINE_RENDERER

#include <GL/glew.h>
#include "mesh.hpp"
#include "camera.hpp"

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

int renderer_skybox_init(RendererSkybox*);
void renderer_skybox_destroy(RendererSkybox*);
void renderer_skybox_render(RendererSkybox*, Camera* camera);

#endif
