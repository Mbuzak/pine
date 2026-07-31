#include "outline.hpp"

void shader_outline_init(Shader* shader) {
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

	shader_init(shader, "outline", COUNT, UNIFORM_IDS, UNIFORM_NAMES);
}

void shader_outline_render(Shader* shader, int selected_id, Transform* transform, Mesh* mesh) {
	glm::mat4 model = transform_model_compute(transform);
	model = glm::scale(model, glm::vec3(1.35, 1.04, 1.35));

	glStencilMask(0x00);
	glStencilFunc(GL_NOTEQUAL, selected_id + 1, 0xFF);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(shader->id);
	glUniformMatrix4fv(shader->locations[UNIFORM_MODEL], 1, GL_FALSE, glm::value_ptr(model));
	mesh_texture_draw(mesh);

	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glStencilMask(0xFF);
}
