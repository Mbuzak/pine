#ifndef PINE_MESH
#define PINE_MESH

#include "cglm/struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <string>
#include <vector>

enum MeshRawBuffer {
	MESH_RAW_POSITIONS,
	MESH_RAW_INDICES,
	MESH_RAW_COUNT
};

enum MeshTextureBuffer {
	MESH_TEXTURE_POSITIONS,
	MESH_TEXTURE_UV_COORDS,
	MESH_TEXTURE_NORMALS,
	MESH_TEXTURE_COUNT
};

struct Mesh {
	GLuint vao;
	GLuint* vbos;
	int size;
};

void mesh_terrain_init(Mesh*);

int mesh_raw_init(Mesh*);
int mesh_texture_init(Mesh*, const char*);

void mesh_raw_draw(Mesh*);
void mesh_texture_draw(Mesh*);

bool load_obj(const char*, std::vector<vec3s>&, std::vector<vec2s>&, std::vector<vec3s>&);

#endif
