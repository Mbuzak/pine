#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>

GLuint program_init(std::string);
void program_destroy(GLuint);

void LinkAndValidateProgram(GLuint);
void CheckForErrors_Program(GLuint, GLenum);
GLuint LoadShader(GLuint, const char*);
void CheckForErrors_Shader(GLuint);
unsigned long getFileLength(std::ifstream&);
GLchar* LoadShaderFile(const char*);

#endif
