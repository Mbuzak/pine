#ifndef PINE_RENDERER
#define PINE_RENDERER

#include <GL/glew.h>
#include "mesh.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"

class RendererSkybox {
public:
	RendererSkybox() = default;
	~RendererSkybox();
	void init();
	void render(Camera*);

private:
	GLuint program_id;
	GLuint texture_id;
	Skybox skybox;
};

#endif
