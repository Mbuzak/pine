#ifndef RENDERED_HPP
#define RENDERED_HPP

#include "cglm/struct.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "entity.hpp"
#include "uniform.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "shader.h"
#include "texture.h"
#ifdef __cplusplus
}
#endif

enum LocationRendered {
	LOCATION_RENDERED_PROJECTION,
	LOCATION_RENDERED_VIEW,
	LOCATION_RENDERED_CAMERA_POSITION,
	LOCATION_RENDERED_COUNT
};

struct ShaderRendered {
	GLuint id;
	GLuint locations[LOCATION_RENDERED_COUNT];
};

void shader_rendered_init(ShaderRendered*, glm::mat4);

void shader_rendered_projection_send(ShaderRendered*, glm::mat4);
void shader_rendered_view_send(ShaderRendered*, mat4s);
void shader_rendered_camera_position_send(ShaderRendered*, vec3s);

void rendered_shape_render(ShaderRendered*, Shape*);

#endif
