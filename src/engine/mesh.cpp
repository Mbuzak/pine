#include "mesh.hpp"
#include "stb_image.h"

Mesh mesh_raw_init() {
	Mesh mesh;
	mesh.type = MESH_RAW;
	mesh.size = 36;

	GLfloat positions[8*3] = {
		1.0f, 1.0f, 1.0f,   // 0
		-1.0f, 1.0f, 1.0f,  // 1
		-1.0f, -1.0f, 1.0f, // 2
		1.0f, -1.0f, 1.0f,  // 3
		1.0f, 1.0f, -1.0f,  // 4
		-1.0f, 1.0f, -1.0f, // 5
		-1.0f, -1.0f, -1.0f,// 6
		1.0f, -1.0f, -1.0f  // 7
	};

	GLuint indices[12*3] = {
		5, 0, 1, 5, 4, 0,
		2, 0, 3, 2, 1, 0,
		7, 0, 4, 7, 3, 0,
		3, 6, 2, 3, 7, 6,
		1, 2, 6, 1, 6, 5,
		4, 5, 6, 4, 6, 7
	};

	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);

	mesh.vbos = new GLuint[MESH_RAW_COUNT];
	glGenBuffers(MESH_RAW_COUNT, mesh.vbos);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[MESH_RAW_POSITIONS]);
	glBufferData(GL_ARRAY_BUFFER, 8*3*sizeof(GLfloat), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[MESH_RAW_INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.size * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	
	return mesh;
}

void mesh_raw_draw(Mesh* mesh) {
	glBindVertexArray(mesh->vao);
	glDrawElements(GL_TRIANGLES, mesh->size, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void mesh_texture_draw(Mesh* mesh) {
	glBindVertexArray(mesh->vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh->size);
	glBindVertexArray(0);
}

Mesh mesh_texture_init(std::string filename) {
	Mesh mesh;
	mesh.type = MESH_TEXTURE;

	std::string path = "models/" + filename + ".obj";
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	if (!loadOBJ(path.c_str(), vertices, uvs, normals)) {
		std::cout << "Error: Model path: " << path << " not exists!\n";
		exit(0);
	}
	mesh.size = vertices.size();

	// Create VAO
	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);

	mesh.vbos = new GLuint[MESH_TEXTURE_COUNT];
	glGenBuffers(MESH_TEXTURE_COUNT, mesh.vbos);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[MESH_TEXTURE_POSITIONS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[MESH_TEXTURE_UV_COORDS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs.size(), &uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[MESH_TEXTURE_NORMALS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	return mesh;
}


Model::Model(std::string filename) {
	std::string path = "models/" + filename + ".obj";
  if (!loadOBJ(path.c_str(), vertices, uvs, normals)) {
		std::cout << "Error: Model path: " << path << " not exists!\n";
		exit(0);
	}

	// Create VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(3, vbos);

	// Create VBO coord
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// Create VBO UV
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs.size(), &uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	// Create VBO color
	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Model::Draw() {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);
}
