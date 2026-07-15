#ifndef PINE_FBO
#define PINE_FBO

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

typedef struct {
	GLuint fbo_id;
	GLuint depth_id;
	GLuint texture_id;
	int width;
	int height;
} Frame;

int frame_init(Frame*);

#endif
