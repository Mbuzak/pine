#ifndef PINE_MESH
#define PINE_MESH

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>

#define MESH_RAW 1
#define MESH_TEXTURE 2

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
	int type;
};

int mesh_raw_init(Mesh*);
int mesh_texture_init(Mesh*, std::string);

void mesh_raw_draw(Mesh*);
void mesh_texture_draw(Mesh*);

bool load_obj(const char*, std::vector<glm::vec3>&, std::vector<glm::vec2>&, std::vector<glm::vec3>&);

#endif
