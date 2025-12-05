#include "display.h"

Display display_init(unsigned int width, unsigned int height, const char* name) {
	Display display;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("[error] Init SDL failed!\n");
		exit(0);
	}

	// OpenGL 3.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	display.window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (display.window == NULL) {
		printf("[error] Create SDL window failed!\n");
		exit(0);
	}

	display.gl_context = SDL_GL_CreateContext(display.window);
	if (display.gl_context == NULL) {
		printf("[error] Create SDL window failed!\n");
		exit(0);
	}

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		printf("GLEW Error\n");
		exit(1);
	}

	/*glEnable(GL_DEPTH_TEST);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_STENCIL_TEST);
	glClearStencil(0);*/

	return display;
}

void display_destroy(Display* display) {
	SDL_GL_DeleteContext(display->gl_context);
	SDL_DestroyWindow(display->window);
	SDL_Quit();
}
