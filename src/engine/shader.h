#ifndef PINE_SHADER
#define PINE_SHADER

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Creates program with vertex and fragment shader
 * filename - name for both vertex and fragment shader file
 * [filename].vs/fs */
GLuint program_init(const char*);
void program_destroy(GLuint);

GLuint shader_load(GLuint, const char*, const char*);
GLchar* shader_code_load(const char*);
void shader_path_get(char*, const char*, const char*);

void program_error_check(GLuint, GLenum);

#endif
