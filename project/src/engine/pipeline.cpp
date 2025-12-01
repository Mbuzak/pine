#include "pipeline.hpp"


Pipeline::Pipeline(std::string vertex, std::string fragment) {
	id_ = glCreateProgram();

	glAttachShader(id_, LoadShader(GL_VERTEX_SHADER, ("shaders/" + vertex).c_str()));
	glAttachShader(id_, LoadShader(GL_FRAGMENT_SHADER, ("shaders/" + fragment).c_str()));

	LinkAndValidateProgram(id_);
}

Pipeline::~Pipeline() {
	glDeleteProgram(id_);
}

void Pipeline::Activate() {
	glUseProgram(id_);
}

void Pipeline::Disactivate() {
	glUseProgram(0);
}
