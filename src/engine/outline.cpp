#include "outline.hpp"

void shader_outline_init(ShaderOutline* shader, glm::mat4 projection) {
	shader->id = program_init("outline");
	shader->locations[LOCATION_OUTLINE_PROJECTION] =
		glGetUniformLocation(shader->id, "matProj");
	shader->locations[LOCATION_OUTLINE_VIEW] =
		glGetUniformLocation(shader->id, "matView");
	shader->locations[LOCATION_OUTLINE_MODEL] =
		glGetUniformLocation(shader->id, "matModel");

	glUseProgram(shader->id);
	shader_outline_projection_send(shader, projection);
	glUseProgram(0);
}

void shader_outline_projection_send(ShaderOutline* shader,
	glm::mat4 projection)
{
	glUniformMatrix4fv(shader->locations[LOCATION_OUTLINE_PROJECTION], 1,
		GL_FALSE, glm::value_ptr(projection));
}

void shader_outline_view_send(ShaderOutline* shader, mat4s view) {
	glUniformMatrix4fv(shader->locations[LOCATION_OUTLINE_VIEW], 1,
		GL_FALSE, view.raw[0]);
}

void shader_outline_model_send(ShaderOutline* shader, glm::mat4 model) {
	glUniformMatrix4fv(shader->locations[LOCATION_OUTLINE_MODEL], 1,
		GL_FALSE, glm::value_ptr(model));
}

void shader_outline_render(ShaderOutline* shader, mat4s view, int selected_id, Transform* transform, Mesh* mesh) {
	glm::mat4 model = transform_model_compute(transform);
	model = glm::scale(model, glm::vec3(1.35, 1.04, 1.35));

	glStencilMask(0x00);
	glStencilFunc(GL_NOTEQUAL, selected_id + 1, 0xFF);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(shader->id);
	shader_outline_view_send(shader, view);
	shader_outline_model_send(shader, model);
	mesh_texture_draw(mesh);

	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glStencilMask(0xFF);
}
