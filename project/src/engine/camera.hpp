#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	void Update();
	void SendUniform(GLuint);

	glm::mat4 perspective;
	glm::mat4 view;

	// Zmienne kontrolujace polozenie i orientacje kamery
	glm::vec2 rot;
	glm::vec3 pos;

	glm::mat4 UpdateViewMatrix();

private:
	glm::vec3 position_;
};

glm::vec3 ExtractCameraPos(const glm::mat4&);


#endif
