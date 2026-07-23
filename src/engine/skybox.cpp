#include "skybox.hpp"

void shader_skybox_init(ShaderSkybox* shader) {
	shader->id = program_init("skybox");
	shader->locations[LOCATION_SKYBOX_PROJECTION] =
		glGetUniformLocation(shader->id, "projection");
	shader->locations[LOCATION_SKYBOX_VIEW] =
		glGetUniformLocation(shader->id, "view");
	shader->locations[LOCATION_SKYBOX_MODEL] =
		glGetUniformLocation(shader->id, "model");
	shader->locations[LOCATION_SKYBOX_TEXTURE] =
		glGetUniformLocation(shader->id, "tex_skybox");
}

void shader_skybox_projection_send(ShaderSkybox* shader, glm::mat4 projection) {
	glUniformMatrix4fv(shader->locations[LOCATION_SKYBOX_PROJECTION],
		1, GL_FALSE, glm::value_ptr(projection));
}

void shader_skybox_view_send(ShaderSkybox* shader, glm::mat4 view) {
	glUniformMatrix4fv(shader->locations[LOCATION_SKYBOX_VIEW],
		1, GL_FALSE, glm::value_ptr(view));
}

void shader_skybox_model_send(ShaderSkybox* shader, glm::mat4 model) {
	glUniformMatrix4fv(shader->locations[LOCATION_SKYBOX_MODEL],
		1, GL_FALSE, glm::value_ptr(model));
}

void shader_skybox_texture_send(ShaderSkybox* shader, GLuint texture_id) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	glUniform1i(shader->locations[LOCATION_SKYBOX_TEXTURE], 0);
}

void renderer_skybox_init(RendererSkybox* renderer, glm::mat4 projection) {
	shader_skybox_init(&renderer->shader);
	mesh_raw_init(&renderer->mesh);
	renderer->texture_id = texture_cube_map_init();

	glUseProgram(renderer->shader.id);
	shader_skybox_projection_send(&renderer->shader, projection);
	glUseProgram(0);
}

void renderer_skybox_destroy(RendererSkybox* renderer) {
	program_destroy(renderer->shader.id);
}

void renderer_skybox_render(RendererSkybox* renderer, glm::mat4 view) {
	const float size = 80.0;
	glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(size));

	glUseProgram(renderer->shader.id);
	shader_skybox_view_send(&renderer->shader, view);
	shader_skybox_model_send(&renderer->shader, scale);
	shader_skybox_texture_send(&renderer->shader, renderer->texture_id);
	mesh_raw_draw(&renderer->mesh);
	glUseProgram(0);
}
