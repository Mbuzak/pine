#include "mesh.hpp"
#include "stb_image.h"

void Skybox::init() {
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

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(COUNT, vbos);

	glBindBuffer(GL_ARRAY_BUFFER, vbos[POSITIONS]);
	glBufferData(GL_ARRAY_BUFFER, 8*3*sizeof(GLfloat), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12*3*sizeof(GLuint), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Skybox::draw() {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
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
