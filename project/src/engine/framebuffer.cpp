#include "framebuffer.hpp"

void Framebuffer::init() {
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	// Create texture
	glGenTextures(1, &id_texture);
	glBindTexture(GL_TEXTURE_2D, id_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, id_texture, 0);

	// Create RBO
	glGenRenderbuffers(1, &id_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, id_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, id_depth);

	// Create summary
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Error: Framebuffer is not complete!\n");
		exit(1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
