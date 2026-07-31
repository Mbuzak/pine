#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include "cglm/struct.h"
#include <GL/glew.h>
#include "mesh.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "shader.h"
#include "texture.h"
#ifdef __cplusplus
}
#endif

struct RendererSkybox {
	Shader* shader;
	Mesh mesh;
	GLuint texture_id;
};

void shader_skybox_init(Shader*);

void renderer_skybox_init(RendererSkybox*, Shader*);
void renderer_skybox_destroy(RendererSkybox*);
void renderer_skybox_render(RendererSkybox*);

#endif
