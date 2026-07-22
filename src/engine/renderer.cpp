#include "renderer.hpp"

void renderer_static_init(RendererStatic* renderer) {
	shader_static_init(&renderer->shader, program_init("default"));
	renderer->fov = 60.0;
	renderer->plane_near = 0.1;
	renderer->plane_far = 200.0;
}

void solid_render(GLuint program_id, Transform* transform, Mesh* mesh) {
	glm::mat4 mat_model = transform_model_compute(transform);
	glUniformMatrix4fv(glGetUniformLocation(program_id, "matModel"), 1, GL_FALSE, glm::value_ptr(mat_model));

	glm::mat3 matNormal = glm::transpose(glm::inverse(mat_model));
	glUniformMatrix3fv(glGetUniformLocation(program_id, "matNormal"), 1, GL_FALSE, glm::value_ptr(matNormal));

	mesh_texture_draw(mesh);
}

void shape_render(GLuint program_id, Shape* shape) {
	glm::mat4 model = transform_model_compute(&shape->transform);
	uniform_mat4f_send(program_id, "matModel", model);

	glm::mat3 matNormal = glm::transpose(glm::inverse(model));
	glUniformMatrix3fv(glGetUniformLocation(program_id, "matNormal"), 1, GL_FALSE, glm::value_ptr(matNormal));

	uniform_material_send(program_id, "my_material.", &shape->material);

	bool has_texture = shape->texture_ != -1;
	glUniform1i(glGetUniformLocation(program_id, "hasTex"), has_texture);
	if (has_texture) {
		texture_2d_send(program_id, shape->texture_);
	}

	mesh_texture_draw(shape->mesh);
}

void outline_render(GLuint program_id, int selected_id, Transform* transform,
	Mesh* mesh)
{
	glStencilMask(0x00);
	glStencilFunc(GL_NOTEQUAL, selected_id + 1, 0xFF);
	glDisable(GL_DEPTH_TEST);

	 // Display
	glm::mat4 model = transform_model_compute(transform);
	model = glm::scale(model, glm::vec3(1.35, 1.04, 1.35));
	uniform_mat4f_send(program_id, "matModel", model);
	mesh_texture_draw(mesh);

	glEnable(GL_DEPTH_TEST);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glStencilMask(0xFF);
}
