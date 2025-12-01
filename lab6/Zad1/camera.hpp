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
    Camera();

    glm::mat4 getMatView();
    glm::vec3 getPosition();

    void update();

private:
    glm::mat4 mMatView;
    glm::vec3 mPosition;
};

#endif
