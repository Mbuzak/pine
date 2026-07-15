#include "frame.h"

int frame_init(Frame* frame) {
	frame->width = 2048;
	frame->height = 1024;

	glGenFramebuffers(1, &frame->fbo_id);
	glBindFramebuffer(GL_FRAMEBUFFER, frame->fbo_id);

	// Create texture
	glGenTextures(1, &frame->texture_id);
	glBindTexture(GL_TEXTURE_2D, frame->texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame->width, frame->height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, frame->texture_id, 0);

	// Create RBO
	glGenRenderbuffers(1, &frame->depth_id);
	glBindRenderbuffer(GL_RENDERBUFFER, frame->depth_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
		frame->width, frame->height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, frame->depth_id);

	// Create summary
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Error: Framebuffer is not complete!\n");
		exit(1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
