#include "shader.h"

GLuint program_init(const char* filename) {
	GLuint program_id = glCreateProgram();

	// Create vertex shader
	GLuint shader_vertex = shader_load(GL_VERTEX_SHADER, filename, ".vs");
	glAttachShader(program_id, shader_vertex);

	// Create fragment shader
	GLuint shader_fragment = shader_load(GL_FRAGMENT_SHADER, filename, ".fs");
	glAttachShader(program_id, shader_fragment);

	glLinkProgram(program_id);
	program_error_check(program_id, GL_LINK_STATUS);

	// Shaders can be removed after linking to program
	glDetachShader(program_id, shader_vertex);
	glDeleteShader(shader_vertex);
	glDetachShader(program_id, shader_fragment);
	glDeleteShader(shader_fragment);

	glValidateProgram(program_id);
	program_error_check(program_id, GL_VALIDATE_STATUS);

	return program_id;
}

void program_destroy(GLuint program_id) {
	glDeleteProgram(program_id);
}

GLuint shader_load(GLuint type, const char* filename, const char* extension) {
	GLuint shader = glCreateShader(type);

	char path[32];
	strcpy(path, "res/shaders/");
	strcat(path, filename);
	strcat(path, extension);

	char* code = shader_code_load(path);
	glShaderSource(shader, 1, (const char**)&code, NULL);
	glCompileShader(shader);
	free(code);

	// Shader error check
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		GLint log_len;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
		char* log = malloc(sizeof(char) * log_len);
		glGetShaderInfoLog(shader, log_len, NULL, log);
		printf("LOG: %s\n", log);
		free(log);
	}

	return shader;
}

GLchar* shader_code_load(const char* path) {
	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Can't open file %s !\n", path);
		exit(1);
	}

	// Get file length
	fseek(file, 0, SEEK_END);
	unsigned long len = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (len == 0) {
		printf("File %s is empty!\n", path);
		exit(1);
	};

	char* code = malloc(sizeof(GLchar) * len+1);
	if (code == NULL) {
		printf("Can't reserve memory %ld \n", len+1);
		exit(1);
	}

	// len isn't always strlen cause some characters are stripped in ascii read...
	// it is important to 0-terminate the real length later, len is just max possible value...
	code[len] = 0;

	unsigned int i=0;
	char ch;
	while ((ch = fgetc(file)) != EOF) {
		code[i] = ch;
		i++;
	}

	/*code[i] = 0;*/
	fclose(file);
	return code;
}

void program_error_check(GLuint program, GLenum mode) {
	GLint status;
	glGetProgramiv(program, mode, &status);

	if (status != GL_TRUE) {
		GLint log_len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
		char* log = malloc(sizeof(char) * log_len);
		glGetProgramInfoLog(program, log_len, NULL, log);
		printf("LOG: %s\n", log);
		free(log);
	}
}

void shader_init(Shader* shader, const char* name, const int count,
				 const int* uniform_ids, const char uniform_names[][32]) {
	shader->id = program_init(name);
	shader->locations = malloc(sizeof(GLuint) * UNIFORM_COUNT);

	for (int i = 0; i < UNIFORM_COUNT; i++) {
		shader->locations[i] = -1;
	}

	for (int i = 0; i < count; i++) {
		shader->locations[uniform_ids[i]] =
			glGetUniformLocation(shader->id, uniform_names[i]);
	}
}

void uniform_mat4_send(Shader* shaders, int count, int uniform_id, mat4s mat) {
	for (int i = 0; i < count; i++) {
		int location = shaders[i].locations[uniform_id];
		if (location != -1) {
			glUseProgram(shaders[i].id);
			glUniformMatrix4fv(location, 1, GL_FALSE, mat.raw[0]);
			glUseProgram(0);
		}
	}
}
