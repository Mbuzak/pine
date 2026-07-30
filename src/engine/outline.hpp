#ifndef OUTLINE_HPP
#define OUTLINE_HPP

#include "cglm/struct.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "entity.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "shader.h"
#include "texture.h"
#ifdef __cplusplus
}
#endif

enum LocationOutline {
	LOCATION_OUTLINE_PROJECTION,
	LOCATION_OUTLINE_VIEW,
	LOCATION_OUTLINE_MODEL,
	LOCATION_OUTLINE_COUNT
};

struct ShaderOutline {
	GLuint id;
	GLuint locations[LOCATION_OUTLINE_COUNT];
};

void shader_outline_init(ShaderOutline*, glm::mat4);
void shader_outline_projection_send(ShaderOutline*, glm::mat4);
void shader_outline_view_send(ShaderOutline*, mat4s);
void shader_outline_model_send(ShaderOutline*, glm::mat4);
void shader_outline_render(ShaderOutline*, mat4s, int, Transform*, Mesh*);

#endif
