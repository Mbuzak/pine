#ifndef PINE_MESH
#define PINE_MESH

#include "cglm/struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <vector>

enum MeshTextureBuffer {
	MESH_TEXTURE_POSITIONS,
	MESH_TEXTURE_UV_COORDS,
	MESH_TEXTURE_NORMALS,
	MESH_TEXTURE_COUNT
};

struct Mesh {
	GLuint vao;
	GLuint* vbos;
	GLuint ibo;
	int size;
};

void mesh_terrain_init(Mesh*);

int mesh_texture_init(Mesh*, const char*);

void mesh_raw_draw(Mesh*);
void mesh_texture_draw(Mesh*);

bool load_obj(const char*, std::vector<vec3s>&, std::vector<vec2s>&, std::vector<vec3s>&);

void attribute_vec2_enable(Mesh* mesh, GLuint index, int size, vec2s* data);
void attribute_vec3_enable(Mesh* mesh, GLuint index, int size, vec3s* data);
void ibo_enable(Mesh* mesh, GLint ibo_size, GLuint* indices);

#endif
