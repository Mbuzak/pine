#ifndef PINE_TERRAIN_H
#define PINE_TERRAIN_H

#include "cglm/struct.h"
#include "entity.h"
#include "frame.h"
#include "texture.h"
#include "display.h"
#include "shader.h"

typedef struct {
	vec2s position;
	Mesh mesh;
	Material material;
	GLuint texture_id;
	Shader* shader;

	Frame frame;
} RendererTerrain;

void shader_terrain_init(Shader* shader);

void renderer_terrain_init(RendererTerrain* renderer, Shader* shader);

void renderer_terrain_render(RendererTerrain* renderer);

#endif
