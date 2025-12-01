#include "camera.hpp"

Camera::Camera() {}

glm::mat4 Camera::getMatView() { return mMatView; }

glm::vec3 Camera::getPosition() { return mPosition; }

void Camera::update() {
    // ------------------------------------------------------------
	// Wyliczanie macierz widoku
	// ------------------------------------------------------------
	mMatView = UpdateViewMatrix();

	// -----------------------------------------------
	// NOWE : wyliczanie pozycji kamery z matView
	// -----------------------------------------------
	mPosition = ExtractCameraPos(mMatView);
}
