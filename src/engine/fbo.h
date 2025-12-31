#ifndef PINE_FBO
#define PINE_FBO

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

typedef struct {
	GLuint id;
	GLuint id_depth;
	GLuint id_texture;
	
	int width;
	int height;
} Framebuffer;

int fbo_init(Framebuffer*);

#endif
