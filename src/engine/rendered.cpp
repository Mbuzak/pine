#include "rendered.hpp"

void shader_rendered_init(Shader* shader) {
	const int COUNT = 7;
	const int UNIFORM_IDS[COUNT] = {
		UNIFORM_PROJECTION,
		UNIFORM_VIEW,
		UNIFORM_MODEL,
		UNIFORM_LIGHT_PROJECTION,
		UNIFORM_LIGHT_VIEW,
		UNIFORM_SHADOW_TEXTURE,
		UNIFORM_CAMERA_COORDS,
	};
	char UNIFORM_NAMES[COUNT][32] = {
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

void rendered_shape_render(Shader* shader, Shape* shape) {
	mat4s model = transform_model_compute(&shape->transform);
	glUniformMatrix4fv(glGetUniformLocation(shader->id, "matModel"), 1, GL_FALSE, model.raw[0]);
	uniform_material_send(shader->id, "my_material.", &shape->material);
	texture_2d_send(shader->id, shape->texture_);
	mesh_texture_draw(shape->mesh);
}
