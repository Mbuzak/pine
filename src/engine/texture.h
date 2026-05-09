#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>

#define ALPHA_CHANNEL 4

GLuint texture_2d_init(const char*);
GLuint texture_cube_map_init();

void texture_2d_send(GLuint, GLuint);
void texture_cube_map_send(GLuint, GLuint);

#endif
