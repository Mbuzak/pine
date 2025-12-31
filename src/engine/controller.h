#ifndef PINE_CONTROLLER
#define PINE_CONTROLLER

#include <SDL2/SDL.h>

typedef struct {
	SDL_Point mouse_pos;
	int mouse_button_left;
	int mouse_button_right;
} Controller;

#endif
