#ifndef PINE_DISPLAY
#define PINE_DISPLAY

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>

typedef struct {
	unsigned int width;
	unsigned int height;
	SDL_Window* window;
	SDL_GLContext gl_context;
} Display;

Display display_init(unsigned int, unsigned int, const char*);

void display_destroy(Display*);

#endif
