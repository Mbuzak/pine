#include "shadow.hpp"

void shader_shadow_map_init(Shader* shader) {
	const int COUNT = 3;
	const int UNIFORM_IDS[COUNT] = {
		UNIFORM_LIGHT_PROJECTION,
		UNIFORM_LIGHT_VIEW,
		UNIFORM_MODEL,
	};
	char UNIFORM_NAMES[COUNT][32] = {
		"lightProj",
		"lightView",
		"matModel",
	};

	shader_init(shader, "shadow_map", COUNT, UNIFORM_IDS, UNIFORM_NAMES);
}

void ShadowMap::Init(Shader* shader) {
	this->shader = shader;

	// Create texture
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//float borderColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Create depth frameBuffer
	glGenFramebuffers(1, &fbo_id);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, texture_id, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::Render(std::vector<Shape> pieces) {
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader->id);
	for (Shape piece: pieces) {
		glUniformMatrix4fv(shader->locations[UNIFORM_MODEL],
			1, GL_FALSE, glm::value_ptr(transform_model_compute(&piece.transform)));
		mesh_texture_draw(piece.mesh);
	}
	glUseProgram(0);
}

void ShadowMap::SendTexture(GLuint program_id) {
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glUniform1i(glGetUniformLocation(program_id, "tex_shadowMap"), 2);
}
