#ifndef PINE_SHADOW
#define PINE_SHADOW

#include "cglm/struct.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "entity.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "shader.h"
#ifdef __cplusplus
}
#endif

enum LocationShadowMap {
	LOCATION_SHADOW_MAP_LIGHT_PROJECTION,
	LOCATION_SHADOW_MAP_LIGHT_VIEW,
	LOCATION_SHADOW_MAP_MODEL,
	LOCATION_SHADOW_MAP_COUNT
};

struct ShaderShadowMap {
	GLuint id;
	GLuint locations[LOCATION_SHADOW_MAP_COUNT];
};

void shader_shadow_map_init(ShaderShadowMap*, glm::mat4, glm::mat4);

void shader_shadow_map_light_projection_send(ShaderShadowMap*, glm::mat4);
void shader_shadow_map_light_view_send(ShaderShadowMap*, mat4s);

// Shadow map of direction or point light
class ShadowMap {
public:
	void Init(glm::mat4, mat4s);

	// Render shapes to depth map
	void Render(std::vector<Shape>);

	void SendTexture(GLuint);

public:
	const unsigned int width = 1024;
	const unsigned int height = 1024;

	ShaderShadowMap shader;
	GLuint fbo_id;
	GLuint texture_id;
};

#endif
