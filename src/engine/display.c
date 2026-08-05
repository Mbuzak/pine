#include "display.h"

Display display_init(unsigned int width, unsigned int height, const char* name) {
	Display display;
	display.width = width;
	display.height = height;
	
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

	display.window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
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

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.7f, 0.7f, 1.0f, 1.0f);
	glEnable(GL_STENCIL_TEST);
	glClearStencil(0);

	/* Value of stencil buffer will be replaced only in case of
	positive pass stencil and depth test */
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	return display;
}

void display_destroy(Display* display) {
	SDL_GL_DeleteContext(display->gl_context);
	SDL_DestroyWindow(display->window);
	SDL_Quit();
}

vec3s screen_to_world_space_convert(int x, int y, SDL_Window* window, mat4s view) {
	int width, height;
	SDL_GetWindowSize(window, &width, &height);

	GLfloat depth;
	glReadPixels(x, height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

	const mat4s projection = perspective_projection_compute(width, height);
	mat4s world_space = glms_mat4_mul(projection, view);
	const vec3s window_coords = {{(float)x, height - (float)y, depth}};
	const vec4s viewport = {{0, 0, (float)width, (float)height}};
	return glms_unproject(window_coords, world_space, viewport);
}

mat4s perspective_projection_compute(float width, float height) {
	const float FOV = glm_rad(60.0);
	const float ASPECT_RATIO = width / (float)height;
	const float PLANE_NEAR = 0.1;
	const float PLANE_FAR = 200.0;
	return glms_perspective(FOV, ASPECT_RATIO, PLANE_NEAR, PLANE_FAR);
}
