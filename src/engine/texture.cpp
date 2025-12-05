#include "texture.hpp"

#include "stb_image.h"

GLuint texture_2d_init(const char* file) {
	GLuint id;

	return id;
}

Texture::Texture(const char *file) {
	// Load from file
	char path[64] = "textures/";
	strcat(path, file);

	int width, height, n;
	unsigned char* texture;
	texture = stbi_load(path, &width, &height, &n, 0);

	if (texture == NULL) {
		std::cout << "Texture " << path << " not found\n";
	}
	
	// Bind texture
	glGenTextures(1, &id_);
	glBindTexture(GL_TEXTURE_2D, id_);
	
	if (n == ALPHA_CHANNEL)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void Texture::Send(GLuint programID) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id_);
	glUniform1i(glGetUniformLocation(programID, "uTexture"), 0);
}

GLuint texture_cube_map_init() {
	const char files[6][30] = {"skybox/posx.jpg", "skybox/negx.jpg",
	"skybox/posy.jpg", "skybox/negy.jpg", "skybox/posz.jpg", "skybox/negz.jpg"
	};

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	// Texture flip turned off. Why?
	stbi_set_flip_vertically_on_load(false);

	for (int i = 0; i < 6; ++i) {
		int width, height, n;
		unsigned char *texture;

		texture = stbi_load(files[i], &width, &height, &n, 0);
		if (texture == NULL) {
			printf("Image %s can't be loaded!\n", files[i]);
			exit(1);
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);

		stbi_image_free(texture);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// Powrot Flipowanie tekstury
	stbi_set_flip_vertically_on_load(true);

	return id;
}

void texture_cube_map_send(GLuint program_id, GLuint texture_id) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	glUniform1i(glGetUniformLocation(program_id, "tex_skybox"), 0);
}
