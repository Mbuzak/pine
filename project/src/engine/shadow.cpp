#include "shadow.hpp"

ShadowMap::ShadowMap() {}

void ShadowMap::Init(glm::vec3 direction) {
	// Ustawienie polozenia i kierunku swiatla
	//Light_Direction = glm::normalize(glm::vec3(0.2, -0.8f, 1.1f));
	Light_Position = glm::vec3(-10.12f, 2.0f, -10.12f);

	// Ustawienie macierzy kamery swiatla
	float size = 18.0;
	//float size = 30.0;
	lightProj = glm::ortho(-size, size, -size, size, 2.0f, 35.5f);
	lightView = glm::lookAt(Light_Position, Light_Position + direction, glm::vec3(0.0f, 1.0f, 0.0f));

	// Create texture
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width_, height_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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
}

void ShadowMap::Render(std::vector<Piece*> pieces) {
	// Render texture from light poisiton
	glViewport(0, 0, width_, height_);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(program_id);
	// Send camera from light position
	glUniformMatrix4fv(glGetUniformLocation(program_id, "lightProj"), 1, GL_FALSE, glm::value_ptr(lightProj));
	glUniformMatrix4fv(glGetUniformLocation(program_id, "lightView"), 1, GL_FALSE, glm::value_ptr(lightView));

	// Render shapes
	for (Shape *piece: pieces) {
		glUniformMatrix4fv(glGetUniformLocation(program_id, "matModel"), 1, GL_FALSE, glm::value_ptr(piece->CalculateMatModel()));
		piece->model_->Draw();
	}

	glUseProgram(0);
}

void ShadowMap::SendTexture(GLuint program_id) {
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glUniform1i(glGetUniformLocation(program_id, "tex_shadowMap"), 2);
}
