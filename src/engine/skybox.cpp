#include "skybox.hpp"

void shader_skybox_init(Shader* shader) {
	const int COUNT = 4;
	const int UNIFORM_IDS[COUNT] = {
		UNIFORM_PROJECTION,
		UNIFORM_VIEW,
		UNIFORM_MODEL,
		UNIFORM_TEXTURE,
	};
	char UNIFORM_NAMES[COUNT][32] = {
		"projection",
		"view",
		"model",
		"tex_skybox",
	};

	shader_init(shader, "skybox", COUNT, UNIFORM_IDS, UNIFORM_NAMES);
}

void renderer_skybox_init(RendererSkybox* renderer, Shader* shader) {
	renderer->shader = shader;
	mesh_raw_init(&renderer->mesh);
	renderer->texture_id = texture_cube_map_init();
}

void renderer_skybox_destroy(RendererSkybox* renderer) {
	program_destroy(renderer->shader->id);
}

void renderer_skybox_render(RendererSkybox* renderer) {
	const vec3s size = {80.0, 80.0, 80.0};
	mat4s scale = glms_scale(glms_mat4_identity(), size);

	glUseProgram(renderer->shader->id);
	// Send model uniform
	glUniformMatrix4fv(renderer->shader->locations[UNIFORM_MODEL],
		1, GL_FALSE, scale.raw[0]);

	// Send texture uniform
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, renderer->texture_id);
	glUniform1i(renderer->shader->locations[UNIFORM_TEXTURE], 0);
	mesh_raw_draw(&renderer->mesh);
	glUseProgram(0);
}
