#include "rendered.h"

void shader_rendered_init(Shader* shader) {
	const int COUNT = 7;
	const int UNIFORM_IDS[7] = {
		UNIFORM_PROJECTION,
		UNIFORM_VIEW,
		UNIFORM_MODEL,
		UNIFORM_LIGHT_PROJECTION,
		UNIFORM_LIGHT_VIEW,
		UNIFORM_SHADOW_TEXTURE,
		UNIFORM_CAMERA_COORDS,
	};
	char UNIFORM_NAMES[7][32] = {
		"matProj",
		"matView",
		"matModel",
		"lightProj",
		"lightView",
		"tex_shadowMap",
		"cameraPos",
	};

	shader_init(shader, "rendered", COUNT, UNIFORM_IDS, UNIFORM_NAMES);
}

void rendered_shapes_render(Shader* shader, Shape* shapes, int shape_count) {
	glUseProgram(shader->id);
	for (int i = 0; i < shape_count; ++i) {
		mat4s model = transform_model_compute(&shapes[i].transform);
		glUniformMatrix4fv(glGetUniformLocation(shader->id, "matModel"), 1, GL_FALSE, model.raw[0]);
		uniform_material_send(shader->id, "my_material.", &shapes[i].material);
		glStencilFunc(GL_ALWAYS, i + 1, 0xFF);
		mesh_raw_draw(shapes[i].mesh);
	}
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glUseProgram(0);
}
