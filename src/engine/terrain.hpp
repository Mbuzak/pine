#ifndef PINE_TERRAIN_HPP
#define PINE_TERRAIN_HPP

#include "SDL2/SDL.h"
#include "entity.hpp"
#include "uniform.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "frame.h"
#include "texture.h"
#include "display.h"
#ifdef __cplusplus
}
#endif

struct RendererTerrain {
	glm::vec2 position;
	Mesh mesh;
	Material material;
	GLuint texture_id;

	Frame frame;
};

void renderer_terrain_init(RendererTerrain*);

void renderer_terrain_render(RendererTerrain*, GLuint);

#endif
