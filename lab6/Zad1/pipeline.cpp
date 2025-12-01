#include "pipeline.hpp"

Pipeline::~Pipeline() {
	//glDeleteProgram( mId );
}

void Pipeline::createProgram() {
	mId = glCreateProgram();
}

void Pipeline::loadShaders(std::string vertex_shader, std::string fragment_shader) {
	glAttachShader( mId, LoadShader(GL_VERTEX_SHADER, ("shaders/" + vertex_shader).c_str()));
	glAttachShader( mId, LoadShader(GL_FRAGMENT_SHADER, ("shaders/" + fragment_shader).c_str()));

	LinkAndValidateProgram( mId );
}

void Pipeline::sendVertices(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos) {
	glUniformMatrix4fv( glGetUniformLocation(mId, "matProj"), 1, GL_FALSE, glm::value_ptr(projection) );
	glUniformMatrix4fv( glGetUniformLocation(mId, "matView"), 1, GL_FALSE, glm::value_ptr(view) );
	glUniform3fv( glGetUniformLocation( mId, "cameraPos" ), 1, &cameraPos[0] );
	glUniformMatrix4fv( glGetUniformLocation(mId, "matModel"), 1, GL_FALSE, glm::value_ptr(model) );

	glm::mat3 matNormal = glm::transpose( glm::inverse( model ) );
	glUniformMatrix3fv( glGetUniformLocation(mId, "matNormal"), 1, GL_FALSE, glm::value_ptr(matNormal) );
}

void Pipeline::activate() {
	glUseProgram( mId );
}

void Pipeline::disactivate() {
	glUseProgram( 0 );
}
