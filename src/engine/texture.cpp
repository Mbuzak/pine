#include "texture.hpp"

#include "stb_image.h"

Texture::Texture(const char *file) {
	Load(file);
	Bind();
}

void Texture::Send(GLuint programID) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id_);
	glUniform1i(glGetUniformLocation(programID, "uTexture"), 0);
}

void Texture::Load(const char *file) {
	char path[64] = "textures/";
	strcat(path, file);

	data_ = stbi_load(path, &width_, &height_, &n_, 0);

	if (data_ == NULL) {
		std::cout << "Texture " << path << " not found\n";
	}
}

void Texture::Bind() {
  glGenTextures(1, &id_);
	glBindTexture(GL_TEXTURE_2D, id_);
	
	if (n_ == ALPHA_CHANNEL)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_);
	else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, data_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}
