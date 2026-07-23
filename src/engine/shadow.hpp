#ifndef PINE_SHADOW
#define PINE_SHADOW

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "entity.hpp"
#include "camera.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "shader.h"
#ifdef __cplusplus
}
#endif

// Shadow map of direction or point light
class ShadowMap {
public:
	void Init(CameraOrthographic*);

	// Render shapes to depth map
	void Render(CameraOrthographic*, std::vector<Shape>);

	void SendTexture(GLuint);

public:
	const unsigned int width = 1024;
	const unsigned int height = 1024;

	GLuint program_id;
	GLuint fbo_id;
	GLuint texture_id;
};

#endif
