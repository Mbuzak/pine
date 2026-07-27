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

void renderer_terrain_render(RendererTerrain* renderer, GLuint shader_id) {
	glm::mat4 model = glm::translate(glm::mat4(1.0),
		{ renderer->position.x, 0, renderer->position.y });
	uniform_mat4f_send(shader_id, "matModel", model);
	glm::mat3 matNormal = glm::transpose(glm::inverse(model));
	glUniformMatrix3fv(glGetUniformLocation(shader_id, "matNormal"), 1, GL_FALSE, glm::value_ptr(matNormal));
	uniform_material_send(shader_id, "my_material.", &renderer->material);
	texture_2d_send(shader_id, renderer->texture_id);
	mesh_raw_draw(&renderer->mesh);
}
