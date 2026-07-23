#include "rendered.hpp"

void shader_rendered_init(ShaderRendered* shader, glm::mat4 projection) {
	shader->id = program_init("rendered");
	shader->locations[LOCATION_RENDERED_PROJECTION] =
		glGetUniformLocation(shader->id, "matProj");
	shader->locations[LOCATION_RENDERED_VIEW] =
		glGetUniformLocation(shader->id, "matView");

	glUseProgram(shader->id);
	shader_rendered_projection_send(shader, projection);
	glUseProgram(0);
}

void shader_rendered_projection_send(ShaderRendered* shader,
	glm::mat4 projection)
{
	glUniformMatrix4fv(shader->locations[LOCATION_RENDERED_PROJECTION], 1,
		GL_FALSE, glm::value_ptr(projection));
}

void shader_rendered_view_send(ShaderRendered* shader, glm::mat4 view) {
	glUniformMatrix4fv(shader->locations[LOCATION_RENDERED_VIEW], 1,
		GL_FALSE, glm::value_ptr(view));
}

void rendered_shape_render(ShaderRendered* shader, Shape* shape) {
	glm::mat4 model = transform_model_compute(&shape->transform);
	uniform_mat4f_send(shader->id, "matModel", model);
	glm::mat3 matNormal = glm::transpose(glm::inverse(model));
	glUniformMatrix3fv(glGetUniformLocation(shader->id, "matNormal"), 1, GL_FALSE, glm::value_ptr(matNormal));
	uniform_material_send(shader->id, "my_material.", &shape->material);
	texture_2d_send(shader->id, shape->texture_);
	mesh_texture_draw(shape->mesh);
}
