#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP


#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>


class Framebuffer {
  public:
    Framebuffer() = default;

    const int get_width();
    const int get_height();

    void init();
  
  private:
    void Create();
    void CreateTexture();
    void CreateRenderBuffer();
    void CreateSummary();
  
  public:
    GLuint id;
    GLuint id_depth;
    GLuint id_texture;
  
  private:
    const int width_ = 2048;
    const int height_ = 1024;
};

#endif
