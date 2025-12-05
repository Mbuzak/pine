#include "shader.hpp"

GLuint program_init(std::string path) {
	GLuint program_id = glCreateProgram();

	glAttachShader(program_id, LoadShader(GL_VERTEX_SHADER, ("shaders/" + path + ".vs").c_str()));
	glAttachShader(program_id, LoadShader(GL_FRAGMENT_SHADER, ("shaders/" + path + ".fs").c_str()));

	LinkAndValidateProgram(program_id);

	return program_id;
}

void program_destroy(GLuint program_id) {
	glDeleteProgram(program_id);
}

void LinkAndValidateProgram(GLuint program) {
	glLinkProgram(program);
	CheckForErrors_Program(program, GL_LINK_STATUS);

	glValidateProgram(program);
	CheckForErrors_Program(program, GL_VALIDATE_STATUS);
}

void CheckForErrors_Program(GLuint program, GLenum mode) {
	GLint status;
	glGetProgramiv(program, mode, &status);

	if (status != GL_TRUE) {
		GLint log_len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
		char *log = new char[log_len];
		glGetProgramInfoLog(program, log_len, NULL, log);
		printf("LOG: %s\n", log);
		delete[] log;
	}
}

GLuint LoadShader(GLuint MODE, const char *filename) {
	GLuint shader = glCreateShader(MODE);
	GLchar* code = LoadShaderFile(filename);

	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);
	CheckForErrors_Shader(shader);

	delete[] code;
	return shader;
}

void CheckForErrors_Shader(GLuint shader) {
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		GLint log_len;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
		char *log = new char[log_len];
		glGetShaderInfoLog(shader, log_len, NULL, log);
		printf("LOG: %s\n", log);
		delete[] log;
	}
}

unsigned long getFileLength(std::ifstream& file) {
	if(!file.good()) return 0;

	unsigned long pos=file.tellg();
	file.seekg(0,std::ios::end);
	unsigned long len = file.tellg();
	file.seekg(std::ios::beg);

	return len;
}

GLchar* LoadShaderFile(const char* filename) {
	std::ifstream file(filename);
	if (!file) {
		printf("Can't open file %s !\n", filename);
		exit(1);
	}

	unsigned long len = getFileLength(file);
	if (len == 0) {
		printf("File %s is empty!\n", filename);
		exit(1);
	};

	GLchar *ShaderSource = new GLchar[len+1];
	if (ShaderSource == NULL) {
		printf("Can't reserve memory %ld \n", len+1);
		exit(1);
	}

	// len isn't always strlen cause some characters are stripped in ascii read...
	// it is important to 0-terminate the real length later, len is just max possible value...
	ShaderSource[len] = 0;

	unsigned int i=0;
	while (file.good()) {
		ShaderSource[i] = file.get();
		if (!file.eof())
		i++;
	}

	ShaderSource[i] = 0;
	file.close();
	return ShaderSource;
}

