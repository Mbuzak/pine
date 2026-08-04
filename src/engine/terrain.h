#ifndef PINE_TERRAIN_H
#define PINE_TERRAIN_H

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

	Frame frame;
} RendererTerrain;

void renderer_terrain_init(RendererTerrain*);

void renderer_terrain_render(RendererTerrain*, Shader*);

#endif
