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
	model = glm::rotate(model, mAngle.z, glm::vec3(0.0, 0.0, 1.0));
	model = glm::translate(model, mPosition);

	return model;
}

glm::mat4 Mesh::scaledMatModel(float x, float y, float z) {
	return glm::scale(calculateMatModel(), glm::vec3(x, y, z));
}

// ------------------
Pipeline pipeGreen;
Pipeline pipeBrown;

Mesh cone1(0.0, 5.0, 0.0);
Mesh cone2(0.0, 5.0, 0.0);
Mesh cube(0.0, 4.0, 0.0);
Mesh torus(0.0, 0.0, 0.0);


void DisplayScene() {
	// Czyszczenie bufora koloru
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Obliczanie macierzy widoku (pozycja kamery)
	matView = glm::mat4(1.0);
	matView = glm::translate(matView, distance);

	// green pipeline
	pipeGreen.activate();
	pipeGreen.sendVertices(cone1.calculateMatModel(), matView, matProj);
	cone1.draw();

	pipeGreen.sendVertices(cone2.calculateMatModel(), matView, matProj);
	cone2.draw();
	pipeGreen.disactivate();

	// brown pipeline
	pipeBrown.activate();
	pipeBrown.sendVertices(cube.scaledMatModel(0.4, 4.0, 0.4), matView, matProj);
	cube.draw();

	pipeBrown.sendVertices(torus.scaledMatModel(0.5, 5.0, 0.5), matView, matProj);
	torus.draw();

	pipeBrown.disactivate();
	glutSwapBuffers();
}

void Animation( int keyframe ) {
	glutPostRedisplay();

	cone1.updateAngleY(0.01);
	cone2.updateAngleY(0.01);
	torus.updateAngleY(0.01);

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

	cone1.createFromObj("models/cone.obj");
	cone2.createFromObj("models/cone.obj");
	cube.createFromObj("models/cube.obj");
	torus.createFromObj("models/torus.obj");

	// update objects angle
	cone1.updateAngleZ(1.57f);
	cone2.updateAngleZ(-1.57f);
	torus.updateAngleZ(1.57f);

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
			cone1.updateAngleX(0.1);
			cone2.updateAngleX(0.1);
			cube.updateAngleX(0.1);
			torus.updateAngleX(0.1);
			break;

		case 's':
			cone1.updateAngleX(-0.1);
			cone2.updateAngleX(-0.1);
			cube.updateAngleX(-0.1);
			torus.updateAngleX(-0.1);
			break;

		case 'd':
			cone1.updateAngleY(0.1);
			cone2.updateAngleY(0.1);
			cube.updateAngleY(0.1);
			torus.updateAngleY(0.1);
			break;

		case 'a':
			cone1.updateAngleY(-0.1);
			cone2.updateAngleY(-0.1);
			cube.updateAngleY(-0.1);
			torus.updateAngleY(-0.1);
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
