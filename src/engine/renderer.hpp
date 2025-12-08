#ifndef PINE_RENDERER
#define PINE_RENDERER

#include <GL/glew.h>
#include "mesh.hpp"
#include "camera.hpp"
#include "texture.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "shader.h"
#ifdef __cplusplus
}
#endif

class RendererSkybox {
public:
	RendererSkybox() = default;
	~RendererSkybox();
	void init();
	void render(Camera*);

private:
	GLuint program_id;
	GLuint texture_id;
	Mesh mesh;
};

#endif
