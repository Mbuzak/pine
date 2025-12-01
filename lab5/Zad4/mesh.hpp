#ifndef MESH_HPP
#define MESH_HPP


#include "obj_loader.hpp"

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Mesh {
public:
	int width;
	int height;
	int n;
	unsigned char *texture;

	Mesh(float x = 0.0, float y = 0.0, float z = 0.0);
	~Mesh();

	void loadTexture(unsigned char *texture, int w, int h, int n) {
		//texture = stbi_load (path, &width , &height , &n, 0);
        this->texture = texture;
        this->width = w;
        this->height = h;
        this->n = n;

		glGenTextures(1, &mIdVBO_uv);
		glBindTexture(GL_TEXTURE_2D, mIdVBO_uv);
		
		if ( n == 4 ) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	float getPositionY();

	void updateAngleX(float value);
	void updateAngleY(float value);
	void updateAngleZ(float value);
	void updatePositionY(float value);

	void createFromObj(std::string path);
	void draw();

	glm::mat4 calculateMatModel();
	glm::mat4 scaledMatModel(float x, float y, float z);

//private:
	glm::vec3 mPosition;
	glm::vec3 mAngle;

	GLuint mIdVAO;
	GLuint mIdVBO_coord;
	GLuint mIdVBO_color;
	GLuint mIdVBO_uv;

	int frame = 5;

	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec2> mUvsB, mUvs;
	std::vector<glm::vec3> mNormals;

	void createVAO();
	// Creates vertices coordinates buffor
	void createCoordVBO();
	// Creates colro buffor using normals
	void createColorVBO();

	void copyBufferUV() {
		for ( size_t i = 0; i < mUvsB.size(); i++ ) {
			mUvs[i].x = mUvsB[i].x;
			mUvs[i].y = mUvsB[i].y;
		}
	}

	void nextFrame() {
		copyBufferUV();

		frame++;
		frame %= 15;

		for ( auto &pos : mUvs ) {
			pos.x += frame;
			pos.x /= 15;
		}

		glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec2 ) * mUvs.size(), &mUvs[0], GL_STATIC_DRAW );
	}
};

#endif
