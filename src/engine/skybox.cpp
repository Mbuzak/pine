#include "skybox.hpp"

void shader_skybox_init(Shader* shader) {
	shader->id = program_init("skybox");
	shader->locations = new GLuint[UNIFORM_COUNT];
	const int SKYBOX_COUNT = 4;
	const int SKYBOX_UNIFORMS[SKYBOX_COUNT] = {
		UNIFORM_PROJECTION,
		UNIFORM_VIEW,
		UNIFORM_MODEL,
		UNIFORM_TEXTURE,
	};
	char SKYBOX_NAMES[SKYBOX_COUNT][32] = {
		"projection",
		"view",
		"model",
		"tex_skybox",
	};

	for (int i = 0; i < SKYBOX_COUNT; i++) {
		shader->locations[SKYBOX_UNIFORMS[i]] =
			glGetUniformLocation(shader->id, SKYBOX_NAMES[i]);
	}
}

void renderer_skybox_init(RendererSkybox* renderer, glm::mat4 projection) {
	shader_skybox_init(&renderer->shader);
	mesh_raw_init(&renderer->mesh);
	renderer->texture_id = texture_cube_map_init();

	glUseProgram(renderer->shader.id);
	glUniformMatrix4fv(renderer->shader.locations[UNIFORM_PROJECTION],
		1, GL_FALSE, glm::value_ptr(projection));
	glUseProgram(0);
}

void renderer_skybox_destroy(RendererSkybox* renderer) {
	program_destroy(renderer->shader.id);
}

void renderer_skybox_render(RendererSkybox* renderer, glm::mat4 view) {
	const float size = 80.0;
	glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(size));

	glUseProgram(renderer->shader.id);
	// Send view uniform
	glUniformMatrix4fv(renderer->shader.locations[UNIFORM_VIEW],
		1, GL_FALSE, glm::value_ptr(view));

	// Send model uniform
	glUniformMatrix4fv(renderer->shader.locations[UNIFORM_MODEL],
		1, GL_FALSE, glm::value_ptr(scale));

	// Send texture uniform
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, renderer->texture_id);
	glUniform1i(renderer->shader.locations[UNIFORM_TEXTURE], 0);
	mesh_raw_draw(&renderer->mesh);
	glUseProgram(0);
}
