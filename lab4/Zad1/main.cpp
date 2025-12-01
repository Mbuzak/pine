#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_stuff.h"

// ----------------------------------------------
// NOWE: plik naglowkowy z funkcja do plikow OBJ
// ----------------------------------------------
#include "obj_loader.hpp"

// Macierze transformacji i rzutowania
glm::mat4 matProj;
glm::mat4 matView;
glm::mat4 matModel;

glm::vec3 distance(0.0, 0.0, -12.0);

// Graphical pipeline class
class Pipeline {
public:
	Pipeline() = default;
	~Pipeline();

	// Creates new pipeline
	void createProgram();
	void loadShaders(std::string vertex_shader, std::string fragment_shader);
	// Model, view, projection
	void sendVertices(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

	void activate();
	void disactivate();

private:
	GLuint mId;
};

Pipeline::~Pipeline() {
	glDeleteProgram( mId );
}

void Pipeline::createProgram() {
	mId = glCreateProgram();
}

void Pipeline::loadShaders(std::string vertex_shader, std::string fragment_shader) {
	glAttachShader( mId, LoadShader(GL_VERTEX_SHADER, ("shaders/" + vertex_shader).c_str()));
	glAttachShader( mId, LoadShader(GL_FRAGMENT_SHADER, ("shaders/" + fragment_shader).c_str()));

	LinkAndValidateProgram( mId );
}

void Pipeline::sendVertices(glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
	glUniformMatrix4fv( glGetUniformLocation(mId, "matProj"), 1, GL_FALSE, glm::value_ptr(projection) );
	glUniformMatrix4fv( glGetUniformLocation(mId, "matView"), 1, GL_FALSE, glm::value_ptr(view) );
	glUniformMatrix4fv( glGetUniformLocation(mId, "matModel"), 1, GL_FALSE, glm::value_ptr(model) );
}

void Pipeline::activate() {
	glUseProgram( mId );
}

void Pipeline::disactivate() {
	glUseProgram( 0 );
}


class Mesh {
public:
	Mesh(float x = 0.0, float y = 0.0, float z = 0.0);
	~Mesh();

	float getPositionY();

	void updateAngleX(float value);
	void updateAngleY(float value);
	void updatePositionY(float value);

	void createFromObj(std::string path);
	void draw();

	glm::mat4 calculateMatModel();

private:
	glm::vec3 mPosition;
	glm::vec3 mAngle;

	GLuint mIdVAO;
	GLuint mIdVBO_coord;
	GLuint mIdVBO_color;

	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec2> mUvs;
	std::vector<glm::vec3> mNormals;

	void createVAO();
	// Creates vertices coordinates buffor
	void createCoordVBO();
	// Creates colro buffor using normals
	void createColorVBO();
};

Mesh::Mesh(float x, float y, float z):
mPosition(glm::vec3{x, y, z}), mAngle(glm::vec3{0.4, -0.7, 0.0}) {}

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

void Mesh::updatePositionY(float value) {
	mPosition.y += value;
}

void Mesh::createFromObj(std::string path) {
	if ( !loadOBJ(path.c_str(), mVertices, mUvs, mNormals) ) {
		std::cout << "File not loaded!\n";
	}

	// std::cout << "Number of vertices: " << mVertices.size() << std::endl;

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
}

void Mesh::draw() {
	glBindVertexArray( mIdVAO );
	glDrawArrays( GL_TRIANGLES, 0, mVertices.size() );
	glBindVertexArray( 0 );
}

glm::mat4 Mesh::calculateMatModel() {
	glm::mat4 model(1.0);
	model = glm::rotate(model, mAngle.x, glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, mAngle.y, glm::vec3(0.0, 1.0, 0.0));
	model = glm::translate(model, mPosition);

	return model;
}

// ------------------
Pipeline pipeGreen;
Pipeline pipeBrown;

Mesh tower(-1.0, -0.5, 0.0);
Mesh cactus(4, -0.5, -3.0);
Mesh cactus2(-10.0, -0.5, -2.0);
Mesh palm(5.0, -0.5, -5.0);
Mesh terrain(0.0, -0.8, 0.0);

// ---------------------------------------
void DisplayScene() {
	// Czyszczenie bufora koloru
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Obliczanie macierzy widoku (pozycja kamery)
	matView = glm::mat4(1.0);
	matView = glm::translate(matView, distance);

	pipeGreen.activate();

	pipeGreen.sendVertices(cactus.calculateMatModel(), matView, matProj);
	cactus.draw();

	pipeGreen.sendVertices(cactus2.calculateMatModel(), matView, matProj);
	cactus2.draw();

	pipeGreen.disactivate();


	pipeBrown.activate();

	pipeBrown.sendVertices(terrain.calculateMatModel(), matView, matProj);
	terrain.draw();

	pipeBrown.sendVertices(tower.calculateMatModel(), matView, matProj);
	tower.draw();

	pipeBrown.sendVertices(palm.calculateMatModel(), matView, matProj);
	palm.draw();

	pipeBrown.disactivate();

	glutSwapBuffers();
}

// cactus velocity
int side = 1;
float v = 0.0;
float vm = 0.1, vi = 0.005;

void Animation( int keyframe ) {
	glutPostRedisplay();
	
	cactus.updateAngleY(-0.01);
	cactus2.updateAngleY(0.01);

	if ( cactus2.getPositionY() > 3.0 and side == 1 ) {
		v -= vi * 2;

		if ( v < 0.0 ) {
			side *= -1;
		}
	}
	else if ( cactus2.getPositionY() < 1.0 and side == -1 ) {
		v -= vi * 2;

		if ( v < 0.0 ) {
			side *= -1;
		}
	}
	else v += vi;
	
	cactus2.updatePositionY(v * side);

	glutTimerFunc( 1000 / 60, Animation, 0 );
}

// ---------------------------------------
void Initialize() {
	// Ustawianie koloru, ktorym bedzie czyszczony bufor koloru
	glClearColor( 0.5, 0.5, 0.5, 1.0 );
	glEnable(GL_DEPTH_TEST);

	pipeGreen.createProgram();
	pipeGreen.loadShaders("vertex_green.glsl", "fragment_green.glsl");

	pipeBrown.createProgram();
	pipeBrown.loadShaders("vertex_brown.glsl", "fragment_brown.glsl");

	tower.createFromObj("models/tower.obj");
	cactus.createFromObj("models/kaktus.obj");
	cactus2.createFromObj("models/kaktus.obj");
	palm.createFromObj("models/palm.obj");
	terrain.createFromObj("models/terrain.obj");

	Animation(100);
}

// ---------------------------------------
void Reshape( int width, int height ) {
	glViewport( 0, 0, width, height );

	// Obliczanie macierzy rzutowania perspektywicznego
	matProj = glm::perspective(glm::radians(90.0f), width/(float)height, 0.1f, 40.0f );
}

// ---------------------------------------------------
void Keyboard( unsigned char key, int x, int y ) {
    switch( key ) {
		case 27:	// ESC key
			exit(0);
			break;

		case 'w':
			tower.updateAngleX(0.1);
			cactus.updateAngleX(0.1);
			cactus2.updateAngleX(0.1);
			palm.updateAngleX(0.1);
			terrain.updateAngleX(0.1);

			break;

		case 's':
			tower.updateAngleX(-0.1);
			cactus.updateAngleX(-0.1);
			cactus2.updateAngleX(-0.1);
			palm.updateAngleX(-0.1);
			terrain.updateAngleX(-0.1);
			break;

		case 'd':
			tower.updateAngleY(0.1);
			cactus.updateAngleY(0.1);
			cactus2.updateAngleY(0.1);
			palm.updateAngleY(0.1);
			terrain.updateAngleY(0.1);
			break;

		case 'a':
			tower.updateAngleY(-0.1);
			cactus.updateAngleY(-0.1);
			cactus2.updateAngleY(-0.1);
			palm.updateAngleY(-0.1);
			terrain.updateAngleY(-0.1);
			break;
		
		case '=':
			distance.z += 0.1;
			break;
		
		case '-':
			distance.z -= 0.1;
			break;
    }

    glutPostRedisplay();
}




// ---------------------------------------------------
int main( int argc, char *argv[] ) {
	// GLUT
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutInitWindowSize( 500, 500 );
	glutCreateWindow( "Obracanie klawiszami WSAD" );
	glutDisplayFunc( DisplayScene );
	glutReshapeFunc( Reshape );

	// Keyboard
	glutKeyboardFunc( Keyboard );

	// GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if( GLEW_OK != err ) {
		printf("GLEW Error\n");
		exit(1);
	}

	// OpenGL
	if( !GLEW_VERSION_3_2 ) {
		printf("Brak OpenGL 3.2!\n");
		exit(1);
	}

	Initialize();

	glutMainLoop();

	return 0;
}
