#include "terrain.hpp"

void renderer_terrain_init(RendererTerrain* renderer, Mesh* mesh) {
	GLuint texture_id = texture_2d_init("grass.png");
	shape_init(&renderer->shape, {0, 0, 0}, mesh, texture_id);
	renderer->shape.transform.scale = 80.0;

	frame_init(&renderer->frame);
}
