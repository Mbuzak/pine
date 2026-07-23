#include "rendered.hpp"

void shader_rendered_init(ShaderRendered* shader, glm::mat4 projection) {
	shader->id = program_init("default");
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
