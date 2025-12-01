#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utilities.hpp"

// Graphical pipeline class
class Pipeline {
public:
	Pipeline() = default;
	~Pipeline();

	// Creates new pipeline
	void createProgram();
	void loadShaders(std::string vertex_shader, std::string fragment_shader);
	// Model, view, projection
	void sendVertices(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos);

	void activate();
	void disactivate();

//private:
	GLuint mId;
};

#endif
