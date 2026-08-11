#include "outline.h"

void shader_outline_init(Shader* shader) {
	const int COUNT = 1;
	const int UNIFORM_IDS[1] = {
		UNIFORM_MODEL,
	};
	char UNIFORM_NAMES[1][32] = {
		"matModel",
	};

	shader_init(shader, "outline", COUNT, UNIFORM_IDS, UNIFORM_NAMES);
}

void shader_outline_render(Shader* shader, int selected_id, Transform* transform, Mesh* mesh) {
	if (selected_id < 0) {
		return;
	}

	mat4s model = transform_model_compute(transform);
	model = glms_scale(model, glms_vec3_fill(1.1));

	glStencilMask(0x00);
	glStencilFunc(GL_NOTEQUAL, selected_id + 1, 0xFF);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(shader->id);
	glUniformMatrix4fv(shader->locations[UNIFORM_MODEL], 1, GL_FALSE, model.raw[0]);
	mesh_raw_draw(mesh);
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glStencilMask(0xFF);
}
