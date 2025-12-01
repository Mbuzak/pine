#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

#include "utilities.hpp"


// Graphical pipeline class
class Pipeline {
public:
	Pipeline(std::string vertex, std::string fragment);
	~Pipeline();

	void Activate();
	void Disactivate();

//private:
	GLuint id_;
};

#endif
