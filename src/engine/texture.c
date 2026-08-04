#include "texture.h"
#include "stb_image.h"

GLuint texture_2d_init(const char* file) {
	GLuint id;

	// Load from file
	char path[64] = "res/textures/";
	strcat(path, file);

	int width, height, n;
	unsigned char* texture;
	texture = stbi_load(path, &width, &height, &n, 0);

	if (texture == NULL) {
		printf("Texture %s not found\n", path);
	}
	
	// Bind texture
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	
	if (n == ALPHA_CHANNEL)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	return id;
}

void texture_2d_send(GLuint program_id, GLuint texture_id) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glUniform1i(glGetUniformLocation(program_id, "uTexture"), 0);
}
