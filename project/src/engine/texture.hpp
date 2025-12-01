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
  void Load(const char *file);
  void Bind();

private:
  GLuint id_;

  unsigned char *data_;
  int width_;
  int height_;
  int n_;
};

#endif
