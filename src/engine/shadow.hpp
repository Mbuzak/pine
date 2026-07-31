#ifndef PINE_SHADOW
#define PINE_SHADOW

#include "cglm/struct.h"
#include <GL/glew.h>
#include <vector>
#include "entity.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "shader.h"
#ifdef __cplusplus
}
#endif

void shader_shadow_map_init(Shader*);

// Shadow map of direction or point light
class ShadowMap {
public:
	void Init(Shader*);

	// Render shapes to depth map
	void Render(std::vector<Shape>);

	void SendTexture(GLuint);

public:
	const unsigned int width = 1024;
	const unsigned int height = 1024;

	Shader* shader;
	GLuint fbo_id;
	GLuint texture_id;
};

#endif
