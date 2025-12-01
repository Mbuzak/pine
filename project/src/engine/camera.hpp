#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utilities.hpp"


class Camera {
public:
  Camera() = default;

  void Update();
  void SendUniform(GLuint programID);

private:
  glm::vec3 position_;
};

#endif
