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

enum LocationsSkybox {
	LOCATIONS_SKYBOX_PVM,
	LOCATIONS_SKYBOX_TEXTURE,
	LOCATIONS_SKYBOX_COUNT
};

struct ShaderSkybox {
	GLuint id;
	GLuint locations[LOCATIONS_SKYBOX_COUNT];
};

struct RendererSkybox {
	ShaderSkybox shader;
	Mesh mesh;
	GLuint texture_id;
};

void shader_skybox_init(ShaderSkybox*);
void shader_skybox_pvm_send(ShaderSkybox*, glm::mat4);
void shader_skybox_texture_send(ShaderSkybox*, GLuint);

void renderer_skybox_init(RendererSkybox*);
void renderer_skybox_destroy(RendererSkybox*);
void renderer_skybox_render(RendererSkybox*, Camera* camera);

#endif
