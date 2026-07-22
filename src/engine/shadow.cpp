#include "shadow.hpp"

void ShadowMap::Init(CameraOrthographic* camera) {
	// Create texture
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//float borderColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Create depth frameBuffer
	glGenFramebuffers(1, &fbo_id);

	// Link texture to frameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_id, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	program_id = program_init("shadow_map");

	// Send default uniforms
	glUseProgram(program_id);
	glUniformMatrix4fv(glGetUniformLocation(program_id, "lightProj"), 1, GL_FALSE, glm::value_ptr(camera->projection));
	glUniformMatrix4fv(glGetUniformLocation(program_id, "lightView"), 1, GL_FALSE, glm::value_ptr(camera->view));
	glUseProgram(0);
}

void ShadowMap::Render(CameraOrthographic* camera, std::vector<Shape*> pieces) {
	// Render texture from light poisiton
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(program_id);
	for (Shape* piece: pieces) {
		glUniformMatrix4fv(glGetUniformLocation(program_id, "matModel"), 1, GL_FALSE, glm::value_ptr(transform_model_compute(&piece->transform)));
		mesh_texture_draw(piece->mesh);
	}
	glUseProgram(0);
}

void ShadowMap::SendTexture(GLuint program_id) {
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glUniform1i(glGetUniformLocation(program_id, "tex_shadowMap"), 2);
}
