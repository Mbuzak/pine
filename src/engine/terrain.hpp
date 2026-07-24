#ifndef PINE_TERRAIN_HPP
#define PINE_TERRAIN_HPP

#include "SDL2/SDL.h"
#include "entity.hpp"

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
	Shape shape;
	Frame frame;
};

void renderer_terrain_init(RendererTerrain*, Mesh*);

#endif
