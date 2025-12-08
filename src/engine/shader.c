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

	char* path = malloc(sizeof(char) * 32);
	shader_path_get(path, filename, extension);
	char* code = shader_code_load(path);
	free(path);

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
	if (!file) {
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

void shader_path_get(char* path, const char* filename, const char* extension) {
	strcpy(path, "shaders/");
	strcat(path, filename);
	strcat(path, extension);
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
