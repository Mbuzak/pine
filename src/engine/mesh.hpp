#ifndef PINE_MESH
#define PINE_MESH

#include "cglm/struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <vector>

struct Mesh {
	GLuint vao;
	GLuint* vbos;
	GLuint ibo;
	int size;
};

void mesh_cube_init(Mesh* mesh);
void mesh_terrain_init(Mesh*);

void mesh_raw_draw(Mesh*);
void mesh_texture_draw(Mesh*);

void attribute_vec2_enable(Mesh* mesh, GLuint index, int size, vec2s* data);
void attribute_vec3_enable(Mesh* mesh, GLuint index, int size, vec3s* data);
void ibo_enable(Mesh* mesh, GLint ibo_size, GLuint* indices);

#endif
