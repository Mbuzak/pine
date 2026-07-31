#include "rendered.hpp"

void shader_rendered_init(Shader* shader) {
	const int COUNT = 3;
	const int UNIFORM_IDS[COUNT] = {
		UNIFORM_PROJECTION,
		UNIFORM_VIEW,
		UNIFORM_MODEL,
	};
	char UNIFORM_NAMES[COUNT][32] = {
		"matProj",
		"matView",
		"matModel",
	};

	shader_init(shader, "rendered", COUNT, UNIFORM_IDS, UNIFORM_NAMES);
}

void rendered_shape_render(Shader* shader, Shape* shape) {
	glm::mat4 model = transform_model_compute(&shape->transform);
	uniform_mat4f_send(shader->id, "matModel", model);
	glm::mat3 matNormal = glm::transpose(glm::inverse(model));
	glUniformMatrix3fv(glGetUniformLocation(shader->id, "matNormal"), 1, GL_FALSE, glm::value_ptr(matNormal));
	uniform_material_send(shader->id, "my_material.", &shape->material);
	texture_2d_send(shader->id, shape->texture_);
	mesh_texture_draw(shape->mesh);
}
