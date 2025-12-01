#include "mesh.hpp"

Mesh::Mesh(float x, float y, float z):
mPosition(glm::vec3{x, y, z}), mAngle(glm::vec3{0.0, 0.0, 0.0}) {}

Mesh::~Mesh() {
	glDeleteVertexArrays( 1, &mIdVBO_coord );
	glDeleteVertexArrays( 1, &mIdVBO_color );
	glDeleteVertexArrays( 1, &mIdVAO );
}

float Mesh::getPositionY() {
	 return mPosition.y;
}

void Mesh::updateAngleX(float value) {
	mAngle.x += value;
}

void Mesh::updateAngleY(float value) {
	mAngle.y += value;
}

void Mesh::updateAngleZ(float value) {
	mAngle.z += value;
}

void Mesh::updatePositionY(float value) {
	mPosition.y += value;
}

void Mesh::createFromObj(std::string path) {
	if ( !loadOBJ(path.c_str(), mVertices, mUvsB, mNormals) ) {
		std::cout << "File not loaded!\n";
	}

	for ( size_t i = 0; i < mUvsB.size(); i++ ) {
		glm::vec2 tmp;
		tmp.x = mUvsB[i].x;
		tmp.y = mUvsB[i].y;

		mUvs.push_back(tmp);
	}

	//mUvs = mUvsB;
	//copyBufferUV();
	

	for ( auto &pos : mVertices ) {
		std::cout << "(" << pos.x << ", " << pos.y << ", " << pos.z << ")\n";
	}

	std::cout << std::endl;

	for ( auto &pos : mUvs ) {
		pos.x += frame;
		pos.x /= 15;
		std::cout << "(" << pos.x << ", " << pos.y << ")\n";
	}

	createVAO();
}

void Mesh::createVAO() {
	glGenVertexArrays( 1, &mIdVAO );
	glBindVertexArray( mIdVAO );
	
	createCoordVBO();
	createColorVBO();

	glBindVertexArray( 0 );
}

void Mesh::createCoordVBO() {
	glGenBuffers( 1, &mIdVBO_coord );
	glBindBuffer( GL_ARRAY_BUFFER, mIdVBO_coord );
	glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * mVertices.size(), &mVertices[0], GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );
}

void Mesh::createColorVBO() {
	glGenBuffers( 1, &mIdVBO_color );
	glBindBuffer( GL_ARRAY_BUFFER, mIdVBO_color );
	glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * mNormals.size(), &mNormals[0], GL_STATIC_DRAW );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 1 );

	glGenBuffers( 1, &mIdVBO_uv );
	glBindBuffer( GL_ARRAY_BUFFER, mIdVBO_uv );
	glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec2 ) * mUvs.size(), &mUvs[0], GL_STATIC_DRAW );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 2 );
}

void Mesh::draw() {
	glBindVertexArray( mIdVAO );
	glDrawArrays( GL_TRIANGLES, 0, mVertices.size() );
	glBindVertexArray( 0 );
}

glm::mat4 Mesh::calculateMatModel() {
	glm::mat4 model(1.0);
	
	model = glm::translate(model, mPosition);
	/*model = glm::rotate(model, mAngle.x, glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, mAngle.y, glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, mAngle.z, glm::vec3(0.0, 0.0, 1.0));*/
	

	return model;
}

glm::mat4 Mesh::scaledMatModel(float x, float y, float z) {
	return glm::scale(calculateMatModel(), glm::vec3(x, y, z));
}
