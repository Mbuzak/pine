#ifndef SHADOW_HPP
#define SHADOW_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "mesh.hpp"
#include "shader.hpp"

// Shadow map of direction or point light
class ShadowMap {
public:
	// Initialize essential components
	void Init(glm::vec3);

	// Render shapes to depth map
	void Render(std::vector<Piece*>);

	void SendTexture(GLuint);

public:
	const unsigned int width_ = 1024;
	const unsigned int height_ = 1024;

	GLuint program_id;
	GLuint fbo_id;
	GLuint texture_id;
};

// Parametry swiatla kierunkowego, ale znacznie lepiej ubrac
// to w strukture, ktora uzywalismy podczas zajec z oswietlenia
//inline glm::vec3 Light_Direction;
inline glm::vec3 Light_Position;

// Macierze rzutowania dla kamery patrzacej z punktu widzenia oswietlenia
inline glm::mat4 lightProj;
inline glm::mat4 lightView;

#endif
