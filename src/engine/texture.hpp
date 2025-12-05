#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string.h>
#include <GL/glew.h>

#include <iostream>

#define ALPHA_CHANNEL 4


// Visible mesh texture
class Texture {
public:
  Texture(const char *file);
  void Send(GLuint programID);

private:
  GLuint id_;
};

GLuint texture_2d_init(const char*);
GLuint texture_cube_map_init();

void texture_2d_send(GLuint, GLuint);
void texture_cube_map_send(GLuint, GLuint);

#endif
