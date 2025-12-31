#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	void init(glm::vec3, glm::vec2);
	void Update();
	void SendUniform(GLuint);
	void update_perspective(float);

	glm::mat4 perspective;
	glm::mat4 view;

	glm::vec3 pos;
	glm::vec2 rot;

	float fov;
	float near_plane;
	float far_plane;

	glm::mat4 UpdateViewMatrix();

private:
	glm::vec3 position_;
};

glm::vec3 ExtractCameraPos(const glm::mat4&);

#endif
