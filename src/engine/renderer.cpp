#include "renderer.hpp"

void renderer_static_init(RendererStatic* renderer) {
	shader_static_init(&renderer->shader, program_init("default"));
	renderer->fov = 60.0;
	renderer->plane_near = 0.1;
	renderer->plane_far = 200.0;
}

int renderer_skybox_init(RendererSkybox* renderer) {
	renderer->program_id = program_init("skybox");
	mesh_raw_init(&renderer->mesh);

	renderer->texture_id = texture_cube_map_init();
	return 0;
}

void renderer_skybox_destroy(RendererSkybox* renderer) {
	program_destroy(renderer->program_id);
}

void renderer_skybox_render(RendererSkybox* renderer, Camera* camera) {
	const float size = 80.0;
	glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(size));
	glm::mat4 matPVM = camera->projection * camera->view * scale;

	glUseProgram(renderer->program_id);
	glUniformMatrix4fv(glGetUniformLocation(renderer->program_id, "matPVM"),
		1, GL_FALSE, glm::value_ptr(matPVM));
	texture_cube_map_send(renderer->program_id, renderer->texture_id);
	mesh_raw_draw(&renderer->mesh);
	glUseProgram(0);
}

void solid_render(GLuint program_id, Transform* transform, Mesh* mesh) {
	glm::mat4 mat_model = transform_model_compute(transform);
	glUniformMatrix4fv(glGetUniformLocation(program_id, "matModel"), 1, GL_FALSE, glm::value_ptr(mat_model));

	glm::mat3 matNormal = glm::transpose(glm::inverse(mat_model));
	glUniformMatrix3fv(glGetUniformLocation(program_id, "matNormal"), 1, GL_FALSE, glm::value_ptr(matNormal));

	mesh_texture_draw(mesh);
}
