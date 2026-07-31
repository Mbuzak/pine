#include "terrain.hpp"

void renderer_terrain_init(RendererTerrain* renderer) {
	renderer->position = {0.0, 0.0};
	mesh_terrain_init(&renderer->mesh);
	renderer->material = { .ambient = {0.1, 0.1, 0.1},
		.diffuse = {0.7, 0.3, 0.1}, .specular = {0.3, 0.1, 0.1},
		.shininess = 1.0 };
	renderer->texture_id = texture_2d_init("grass.png");

	frame_init(&renderer->frame);
}

void renderer_terrain_render(RendererTerrain* renderer, Shader* shader) {
	vec3s pos = { renderer->position.x, 0, renderer->position.y };
	mat4s model = glms_translate(glms_mat4_identity(), pos);
	glUniformMatrix4fv(shader->locations[UNIFORM_MODEL], 1, GL_FALSE, model.raw[0]);
	uniform_material_send(shader->id, "my_material.", &renderer->material);
	texture_2d_send(shader->id, renderer->texture_id);
	mesh_raw_draw(&renderer->mesh);
}
