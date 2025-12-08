#ifndef PINE_MESH
#define PINE_MESH

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "obj_loader.hpp"

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

Mesh mesh_raw_init();
Mesh mesh_colorful_init();
Mesh mesh_texture_init(std::string);

void mesh_raw_draw(Mesh*);
void mesh_colorful_draw(Mesh*);
void mesh_texture_draw(Mesh*);

// Visible model on scene
class Model {
public:
	Model(std::string);

	void Draw();

private:
	GLuint vao;
	GLuint vbos[3];

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
};

#endif
