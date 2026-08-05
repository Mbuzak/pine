#ifndef PINE_DISPLAY
#define PINE_DISPLAY

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "SDL2/SDL.h"
#include "cglm/struct.h"

typedef struct {
	unsigned int width;
	unsigned int height;
	SDL_Window* window;
	SDL_GLContext gl_context;
} Display;

Display display_init(unsigned int, unsigned int, const char*);
void display_destroy(Display*);

vec3s screen_to_world_space_convert(int x, int y, SDL_Window* window, mat4s view);
mat4s perspective_projection_compute(float, float);

#endif
