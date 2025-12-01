#include "camera.hpp"


void Camera::Update() {
	_view_matrix = UpdateViewMatrix();
	position_ = ExtractCameraPos(_view_matrix);
}

void Camera::SendUniform(GLuint programID) {
	glUniformMatrix4fv(glGetUniformLocation(programID, "matView"), 1, GL_FALSE, glm::value_ptr(_view_matrix));
	glUniform3fv(glGetUniformLocation(programID, "cameraPos"), 1, &position_[0]);
}
