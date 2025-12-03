#include "model.hpp"

#include "stb_image.h"


void Skybox::CreateVAO() {
	// Vertex arrays
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Wspolrzedne wierzchokow	
	glGenBuffers(1, &vbo_coord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_coord);
	glBufferData(GL_ARRAY_BUFFER, 8*3*sizeof(GLfloat), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// Tablica indeksow
	glGenBuffers(1, &vbo_idx);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_idx);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12*3*sizeof(GLuint), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	// Tekstura CUBE_MAP
	glGenTextures(1, &vbo_uv);
	glBindTexture(GL_TEXTURE_CUBE_MAP, vbo_uv);

	// Wylaczanie flipowania tekstury
	stbi_set_flip_vertically_on_load(false);

	// Utworzenie 6 tekstur dla kazdej sciany
	for (int i = 0; i < 6; ++i) {
		int tex_width, tex_height, n;
		unsigned char *tex_data;

		tex_data = stbi_load(files[i], &tex_width, &tex_height, &n, 0);
		if (tex_data == NULL) {
			printf("Image %s can't be loaded!\n", files[i]);
			exit(1);
		}
		// Zaladowanie danych do tekstury OpenGL
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);

		// Zwolnienie pamieci pliku graficznego
		stbi_image_free(tex_data);
	}

	// Przykladowe opcje tekstury
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// Powrot Flipowanie tekstury
	stbi_set_flip_vertically_on_load(true);
}

void Skybox::Draw(GLuint program_id, glm::mat4 matProj, glm::mat4 matView) {
	glm::mat4 matPVM = matProj * matView * glm::scale(glm::mat4(1), glm::vec3(80.0, 80.0, 80.0));
	glUniformMatrix4fv(glGetUniformLocation(program_id, "matPVM"), 1, GL_FALSE, glm::value_ptr(matPVM));

	// Aktywacja tekstury CUBE_MAP
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, vbo_uv);
	glUniform1i(glGetUniformLocation(program_id, "tex_skybox"), 0);

	// Rendering boxa
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}


Model::Model(std::string filename) {
	// Load OBJ
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
