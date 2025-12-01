#include "framebuffer.hpp"


const int Framebuffer::get_width() {
  return width_;
}

const int Framebuffer::get_height() {
  return height_;
}

void Framebuffer::init() {
  Create();
  CreateTexture();
  CreateRenderBuffer();
  CreateSummary();
}

void Framebuffer::Create() {
  glGenFramebuffers(1, &id);
  glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void Framebuffer::CreateTexture() {
  glGenTextures(1, &id_texture);
  glBindTexture(GL_TEXTURE_2D, id_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB,
    GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
    GL_TEXTURE_2D, id_texture, 0);
}

void Framebuffer::CreateRenderBuffer() {
  glGenRenderbuffers(1, &id_depth);
  glBindRenderbuffer(GL_RENDERBUFFER, id_depth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width_, height_);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, id_depth);
}

void Framebuffer::CreateSummary() {
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    printf("Error: Framebuffer is not complete!\n");
    exit(1);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
