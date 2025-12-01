#include "shadow.hpp"


ShadowMap::ShadowMap() {
  depth_map_ = new DepthMap();
}

void ShadowMap::Init(glm::vec3 direction) {
  // Ustawienie polozenia i kierunku swiatla
	//Light_Direction = glm::normalize(glm::vec3(0.2, -0.8f, 1.1f));
	Light_Position = glm::vec3(-10.12f, 2.0f, -10.12f);

	// Ustawienie macierzy kamery swiatla
  float size = 18.0;
  //float size = 30.0;
	lightProj = glm::ortho(-size, size, -size, size, 2.0f, 35.5f);
  lightView = glm::lookAt(Light_Position, Light_Position + direction, glm::vec3(0.0f, 1.0f, 0.0f));

  CreateTexture();
  CreateDepthFrameBuffer();
  LinkTextureToFrameBuffer();
  CreateProgram();
}

void ShadowMap::Render(std::vector<Piece*> pieces) {
  RenderTextureFromLightPoisiton();
  Activate();
  SendCameraFromLightPosition();
  RenderShapes(pieces);
  Disactivate();
}

void ShadowMap::SendTexture(GLuint program_id) {
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, depth_map_->texture_id_);
  glUniform1i(glGetUniformLocation(program_id, "tex_shadowMap"), 2);
}

void ShadowMap::CreateTexture() {
  glGenTextures(1, &depth_map_->texture_id_);
	glBindTexture(GL_TEXTURE_2D, depth_map_->texture_id_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depth_map_->width_, depth_map_->height_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//float borderColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
  float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

void ShadowMap::CreateDepthFrameBuffer() {
  glGenFramebuffers(1, &depth_map_->frame_buffer_id_);
}

void ShadowMap::LinkTextureToFrameBuffer() {
  glBindFramebuffer(GL_FRAMEBUFFER, depth_map_->frame_buffer_id_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map_->texture_id_, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::CreateProgram() {
  depth_map_->program_id_ = glCreateProgram();
	glAttachShader(depth_map_->program_id_, LoadShader(GL_VERTEX_SHADER, "shaders/depthmap-vertex.glsl"));
	glAttachShader(depth_map_->program_id_, LoadShader(GL_FRAGMENT_SHADER, "shaders/depthmap-fragment.glsl"));
	LinkAndValidateProgram(depth_map_->program_id_);
}

void ShadowMap::RenderTextureFromLightPoisiton() {
	glViewport(0, 0, depth_map_->width_, depth_map_->height_);
	glBindFramebuffer(GL_FRAMEBUFFER, depth_map_->frame_buffer_id_);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::Activate() {
  glUseProgram(depth_map_->program_id_);
}

void ShadowMap::SendCameraFromLightPosition() {
  glUniformMatrix4fv(glGetUniformLocation(depth_map_->program_id_, "lightProj"), 1, GL_FALSE, glm::value_ptr(lightProj));
  glUniformMatrix4fv(glGetUniformLocation(depth_map_->program_id_, "lightView"), 1, GL_FALSE, glm::value_ptr(lightView));
}

void ShadowMap::RenderShapes(std::vector<Piece*> pieces) {
  for (Shape *piece: pieces) {
    glUniformMatrix4fv(glGetUniformLocation(depth_map_->program_id_, "matModel"), 1, GL_FALSE, glm::value_ptr(piece->CalculateMatModel()));
    piece->model_->Draw();
  }
}

void ShadowMap::Disactivate() {
  glUseProgram(0);
}
