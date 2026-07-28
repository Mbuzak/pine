#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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

struct RendererSkybox {
	Shader shader;
	Mesh mesh;
	GLuint texture_id;
};

void shader_skybox_init(Shader*);

void renderer_skybox_init(RendererSkybox*, glm::mat4);
void renderer_skybox_destroy(RendererSkybox*);
void renderer_skybox_render(RendererSkybox*, glm::mat4);

#endif
