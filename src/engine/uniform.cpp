#include "uniform.hpp"

void uniform_int_send(GLuint program_id, const char* name, GLint value) {
	glUniform1i(glGetUniformLocation(program_id, name), value);
}

void uniform_vec3f_send(GLuint program_id, const char* name, glm::vec3 value) {
	glUniform3fv(glGetUniformLocation(program_id, name), 1, glm::value_ptr(value));
}

void uniform_mat4f_send(GLuint program_id, const char* name, glm::mat4& value) {
	glUniformMatrix4fv(glGetUniformLocation(program_id, name), 1, GL_FALSE, glm::value_ptr(value));
}
