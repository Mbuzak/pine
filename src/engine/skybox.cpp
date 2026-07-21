#include "skybox.hpp"

void shader_skybox_init(ShaderSkybox* shader) {
	shader->id = program_init("skybox");
	shader->locations[LOCATIONS_SKYBOX_PVM] =
		glGetUniformLocation(shader->id, "matPVM");
	shader->locations[LOCATIONS_SKYBOX_TEXTURE] =
		glGetUniformLocation(shader->id, "tex_skybox");
}

void shader_skybox_pvm_send(ShaderSkybox* shader, glm::mat4 pvm) {
	glUniformMatrix4fv(shader->locations[LOCATIONS_SKYBOX_PVM],
		1, GL_FALSE, glm::value_ptr(pvm));
}

void shader_skybox_texture_send(ShaderSkybox* shader, GLuint texture_id) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	glUniform1i(shader->locations[LOCATIONS_SKYBOX_TEXTURE], 0);
}

void renderer_skybox_init(RendererSkybox* renderer) {
	shader_skybox_init(&renderer->shader);
	mesh_raw_init(&renderer->mesh);
	renderer->texture_id = texture_cube_map_init();
}

void renderer_skybox_destroy(RendererSkybox* renderer) {
	program_destroy(renderer->shader.id);
}

void renderer_skybox_render(RendererSkybox* renderer, Camera* camera) {
	const float size = 80.0;
	glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(size));
	glm::mat4 matPVM = camera->projection * camera->view * scale;

	glUseProgram(renderer->shader.id);
	shader_skybox_pvm_send(&renderer->shader, matPVM);
	shader_skybox_texture_send(&renderer->shader, renderer->texture_id);
	mesh_raw_draw(&renderer->mesh);
	glUseProgram(0);
}
