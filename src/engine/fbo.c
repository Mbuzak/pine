#include "fbo.h"

int fbo_init(Framebuffer* fbo) {
	fbo->width = 2048;
	fbo->height = 1024;

	glGenFramebuffers(1, &fbo->id);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->id);

	// Create texture
	glGenTextures(1, &fbo->id_texture);
	glBindTexture(GL_TEXTURE_2D, fbo->id_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbo->width, fbo->height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, fbo->id_texture, 0);

	// Create RBO
	glGenRenderbuffers(1, &fbo->id_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, fbo->id_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, fbo->width, fbo->height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo->id_depth);

	// Create summary
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Error: Framebuffer is not complete!\n");
		exit(1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
