#include "camera.hpp"

void Camera::Update() {
	view = UpdateViewMatrix();
	position_ = ExtractCameraPos(view);
}

void Camera::SendUniform(GLuint programID) {
	glUniformMatrix4fv(glGetUniformLocation(programID, "matView"), 1, GL_FALSE, glm::value_ptr(view));
	glUniform3fv(glGetUniformLocation(programID, "cameraPos"), 1, &position_[0]);
}

// NOWE: funkcja wyliczajaca pozycje kamery z macierzy widoku
// zrodlo: https://community.khronos.org/t/extracting-camera-position-from-a-modelview-matrix/68031
glm::vec3 ExtractCameraPos(const glm::mat4 & a_modelView) {
  // Get the 3 basis vector planes at the camera origin and transform them into model space.
  //
  // NOTE: Planes have to be transformed by the inverse transpose of a matrix
  //       Nice reference here: http://www.opengl.org/discussion_boards/showthread.php/159564-Clever-way-to-transform-plane-by-matrix
  //
  //       So for a transform to model space we need to do:
  //            inverse(transpose(inverse(MV)))
  //       This equals : transpose(MV) - see Lemma 5 in http://mathrefresher.blogspot.com.au/2007/06/transpose-of-matrix.html
  //
  // As each plane is simply (1,0,0,0), (0,1,0,0), (0,0,1,0) we can pull the data directly from the transpose matrix.
  //
  glm::mat4 modelViewT = transpose(a_modelView);

  // Get plane normals
  glm::vec3 n1(modelViewT[0]);
  glm::vec3 n2(modelViewT[1]);
  glm::vec3 n3(modelViewT[2]);

  // Get plane distances
  float d1(modelViewT[0].w);
  float d2(modelViewT[1].w);
  float d3(modelViewT[2].w);

  // Get the intersection of these 3 planes
  // http://paulbourke.net/geometry/3planes/
  glm::vec3 n2n3 = cross(n2, n3);
  glm::vec3 n3n1 = cross(n3, n1);
  glm::vec3 n1n2 = cross(n1, n2);

  glm::vec3 top = (n2n3 * d1) + (n3n1 * d2) + (n1n2 * d3);
  float denom = dot(n1, n2n3);

  return top / -denom;
}
