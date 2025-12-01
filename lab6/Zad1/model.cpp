#include "model.hpp"

Model::~Model() {}

void Model::load(std::string path) {
    if ( !loadOBJ( ("models/" + path).c_str(), mVertices, mUvs, mNormals) ) {
		std::cout << "File " << path << " not loaded!\n";
	}

    createVAO();
}

void Model::addMesh(Material material, float x, float y, float z) {
    mMeshes.push_back(Mesh(material, x, y, z));
}

void Model::draw() {
    glBindVertexArray( mIdVAO );
    glDrawArrays( GL_TRIANGLES, 0, mVertices.size() );
    glBindVertexArray( 0 );
}

void Model::createVAO() {
    glGenVertexArrays( 1, &mIdVAO );
	glBindVertexArray( mIdVAO );
	
	createVBOCoord();
	createVBOUV();
    createVBOColor();

	glBindVertexArray( 0 );
}

void Model::createVBOCoord() {
    glGenBuffers( 1, &mIdVBO_coord );
	glBindBuffer( GL_ARRAY_BUFFER, mIdVBO_coord );
	glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * mVertices.size(), &mVertices[0], GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );
}

void Model::createVBOUV() {
    glGenBuffers( 1, &mIdVBO_uv );
	glBindBuffer( GL_ARRAY_BUFFER, mIdVBO_uv );
	glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec2 ) * mUvs.size(), &mUvs[0], GL_STATIC_DRAW );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 1 );
}

void Model::createVBOColor() {
    glGenBuffers( 1, &mIdVBO_color );
	glBindBuffer( GL_ARRAY_BUFFER, mIdVBO_color );
	glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * mNormals.size(), &mNormals[0], GL_STATIC_DRAW );
	glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 2 );
}


/*void LightModel::addMesh(Material material, Light light) {
	Mesh m(material, light.Position.x, light.Position.y, light.Position.z);
	m.mLight = light;

    mMeshes.push_back(m);
}*/

void SolidModel::loadTexture(int id, unsigned char *texture, int w, int h, int n) {
	//mMeshes[id].tx = std::make_unique<Texture>(texture, w, h, n);
	mMeshes[id].tx = new Texture(texture, w, h, n);

	glGenTextures(1, &mIdVBO_uv);
	glBindTexture(GL_TEXTURE_2D, mIdVBO_uv);
	
	if ( n == 4 ) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void LightModel::addMesh( glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,	glm::vec3 attenuation, glm::vec3 position, bool isPoint ) {
    mMeshes.push_back( LightMesh( ambient, diffuse, specular, attenuation, position, isPoint ) );
}

void LightModel::draw(Pipeline pipe, glm::mat4 matProj, Camera camera) {
	glUniform1i(glGetUniformLocation(pipe.mId, "isLight"), true);
    for( auto &mesh : mMeshes ) {
        glUniform3fv( glGetUniformLocation( pipe.mId, "lightModelColor" ), 1, &mesh.Diffuse[0] );

        pipe.sendVertices(mesh.calculateMatModel(), camera.getMatView(), matProj, camera.getPosition());

		glBindVertexArray( mIdVAO );
		glDrawArrays( GL_TRIANGLES, 0, mVertices.size() );
		glBindVertexArray( 0 );
    }
}
