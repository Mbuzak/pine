#include "terrain.h"

void shader_terrain_init(Shader* shader) {
	const int COUNT = 7;
	const int UNIFORM_IDS[7] = {
		UNIFORM_PROJECTION,
		UNIFORM_VIEW,
		UNIFORM_MODEL,
		UNIFORM_LIGHT_PROJECTION,
		UNIFORM_LIGHT_VIEW,
		UNIFORM_SHADOW_TEXTURE,
		UNIFORM_CAMERA_COORDS,
	};
	char UNIFORM_NAMES[7][32] = {
		"matProj",
		"matView",
		"matModel",
		"lightProj",
		"lightView",
		"tex_shadowMap",
		"cameraPos",
	};

	shader_init(shader, "terrain", COUNT, UNIFORM_IDS, UNIFORM_NAMES);
}

void renderer_terrain_init(RendererTerrain* renderer, Shader* shader) {
	renderer->shader = shader;

	renderer->position = glms_vec2_fill(0.0);
	mesh_terrain_init(&renderer->mesh);
	Material material = { .ambient = {0.1, 0.1, 0.1},
	.diffuse = {0.7, 0.3, 0.1}, .specular = {0.3, 0.1, 0.1},
	.shininess = 1.0 };
	renderer->material = material;
	renderer->texture_id = texture_2d_init("grass.png");

	frame_init(&renderer->frame);
}

void renderer_terrain_render(RendererTerrain* renderer) {
	vec3s pos = { renderer->position.x, 0, renderer->position.y };
	mat4s model = glms_translate(glms_mat4_identity(), pos);
	glUniformMatrix4fv(renderer->shader->locations[UNIFORM_MODEL], 1, GL_FALSE, model.raw[0]);
	uniform_material_send(renderer->shader->id, "my_material.", &renderer->material);
	texture_2d_send(renderer->shader->id, renderer->texture_id);
	mesh_raw_draw(&renderer->mesh);
}
