#ifndef PINE_SHADOW_H
#define PINE_SHADOW_H

#include "cglm/struct.h"
#include <GL/glew.h>
#include "entity.h"
#include "shader.h"

void shader_shadow_map_init(Shader*);

// Shadow map of direction or point light
typedef struct {
	unsigned int width;
	unsigned int height;

	Shader* shader;
	GLuint fbo_id;
	GLuint texture_id;
} ShadowMap;

void shadow_map_init(ShadowMap* shadow_map, Shader* shader);
void shadow_map_render(ShadowMap* shadow_map, Shape* shapes, int count);
void shadow_map_texture_send(ShadowMap* shadow_map, GLuint program_id);

#endif
