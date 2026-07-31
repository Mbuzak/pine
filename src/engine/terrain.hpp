#ifndef PINE_TERRAIN_HPP
#define PINE_TERRAIN_HPP

#include "entity.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "frame.h"
#include "texture.h"
#include "display.h"
#include "shader.h"
#ifdef __cplusplus
}
#endif

struct RendererTerrain {
	vec2s position;
	Mesh mesh;
	Material material;
	GLuint texture_id;

	Frame frame;
};

void renderer_terrain_init(RendererTerrain*);

void renderer_terrain_render(RendererTerrain*, Shader*);

#endif
