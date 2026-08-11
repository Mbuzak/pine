#include "shadow.h"

void shader_shadow_map_init(Shader* shader) {
	const int COUNT = 1;
	const int UNIFORM_IDS[1] = {
		UNIFORM_MODEL,
	};
	char UNIFORM_NAMES[1][32] = {
		"matModel",
	};

	shader_init(shader, "shadow_map", COUNT, UNIFORM_IDS, UNIFORM_NAMES);
}

void shadow_map_init(ShadowMap* shadow_map, Shader* shader) {
	shadow_map->shader = shader;
	shadow_map->width = 1024;
	shadow_map->height = 1024;

	// Create texture
	glGenTextures(1, &shadow_map->texture_id);
	glBindTexture(GL_TEXTURE_2D, shadow_map->texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_map->width, shadow_map->height, 0,
				 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//float borderColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Create depth frameBuffer
	glGenFramebuffers(1, &shadow_map->fbo_id);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow_map->fbo_id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
						   GL_TEXTURE_2D, shadow_map->texture_id, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void shadow_map_render(ShadowMap* shadow_map, Shape* shapes, int count) {
	glViewport(0, 0, shadow_map->width, shadow_map->height);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow_map->fbo_id);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(shadow_map->shader->id);
	for (int i = 0; i < count; i++) {
		glUniformMatrix4fv(shadow_map->shader->locations[UNIFORM_MODEL],
						   1, GL_FALSE, transform_model_compute(&shapes[i].transform).raw[0]);
		mesh_raw_draw(shapes[i].mesh);
	}
	glUseProgram(0);
}

void shadow_map_texture_send(ShadowMap* shadow_map, GLuint program_id) {
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadow_map->texture_id);
	glUniform1i(glGetUniformLocation(program_id, "tex_shadowMap"), 2);
}
