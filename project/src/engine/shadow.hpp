#ifndef SHADOW_HPP
#define SHADOW_HPP

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "mesh.hpp"

#include "utilities.hpp"


class DepthMap {
public:
	DepthMap() = default;

public:
	GLuint program_id_;
	GLuint frame_buffer_id_;
	GLuint texture_id_;

	const unsigned int width_ = 1024;
	const unsigned int height_ = 1024;
};


// Shadow map of direction or point light
class ShadowMap {
public:
	ShadowMap();

	// Initialize essential components
	void Init(glm::vec3 direction);

	// Render shapes to depth map
	void Render(std::vector<Piece*> pieces);

	void SendTexture(GLuint program_id);

public:
	DepthMap *depth_map_;

private:
	void CreateTexture();
	void CreateDepthFrameBuffer();
	void LinkTextureToFrameBuffer();
	void CreateProgram();

	void RenderTextureFromLightPoisiton();
	void Activate();
	void SendCameraFromLightPosition();
	void RenderShapes(std::vector<Piece*> pieces);
	void Disactivate();
};

// Parametry swiatla kierunkowego, ale znacznie lepiej ubrac
// to w strukture, ktora uzywalismy podczas zajec z oswietlenia
//inline glm::vec3 Light_Direction;
inline glm::vec3 Light_Position;

// Macierze rzutowania dla kamery patrzacej z punktu widzenia oswietlenia
inline glm::mat4 lightProj;
inline glm::mat4 lightView;

#endif
