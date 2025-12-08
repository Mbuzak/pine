#include "renderer.hpp"

RendererSkybox::~RendererSkybox() {
	program_destroy(program_id);
}

void RendererSkybox::init() {
	program_id = program_init("skybox");
	mesh = mesh_raw_init();

	texture_id = texture_cube_map_init();
}

void RendererSkybox::render(Camera* camera) {
	glUseProgram(program_id);
	glm::mat4 matPVM = camera->perspective * camera->view * glm::scale(glm::mat4(1), glm::vec3(80.0, 80.0, 80.0));
	glUniformMatrix4fv(glGetUniformLocation(program_id, "matPVM"), 1, GL_FALSE, glm::value_ptr(matPVM));

	texture_cube_map_send(program_id, texture_id);

	mesh_raw_draw(&mesh);
	glUseProgram(0);
}
