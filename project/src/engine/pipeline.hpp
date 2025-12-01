#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>

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

void LinkAndValidateProgram(GLuint);
void CheckForErrors_Program(GLuint, GLenum);
GLuint LoadShader(GLuint, const char*);
void CheckForErrors_Shader(GLuint);
unsigned long getFileLength(std::ifstream&);
GLchar* LoadShaderFile(const char*);

#endif
