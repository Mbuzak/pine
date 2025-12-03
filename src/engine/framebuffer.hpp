#ifndef FBO_HPP
#define FBO_HPP

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

class Framebuffer {
public:
	void init();
	
	GLuint id;
	GLuint id_depth;
	GLuint id_texture;
	
	const int width = 2048;
	const int height = 1024;
};

#endif
