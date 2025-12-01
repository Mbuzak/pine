#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

// -----------------------------------------
// NOWE: plik do obslugi plikow graficznych
// -----------------------------------------
# define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "pipeline.hpp"
#include "mesh.hpp"

// Macierze transformacji i rzutowania
glm::mat4 matProj = glm::mat4(1.0);
glm::mat4 matView = glm::mat4(1.0);
glm::mat4 matModel = glm::mat4(1.0);




// ------------------
Pipeline pipe;

Mesh cube(0.0, 0.0, 0.0);


void DisplayScene() {
	// Czyszczenie bufora koloru
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	matView = UpdateViewMatrix();

	pipe.activate();

	pipe.sendVertices(cube.calculateMatModel(), matView, matProj);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cube.mIdVBO_uv);
	glUniform1i(glGetUniformLocation(pipe.mId, "tex"), 0);

	cube.draw();

	pipe.disactivate();

	glutSwapBuffers();
}

// ---------------------------------------
void Initialize() {
	// Ustawianie koloru, ktorym bedzie czyszczony bufor koloru
	glClearColor( 0.5, 0.5, 0.5, 1.0 );
	glEnable(GL_DEPTH_TEST);

	pipe.createProgram();
	pipe.loadShaders("vertex.glsl", "fragment.glsl");

	

	stbi_set_flip_vertically_on_load(true);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	cube.createFromObj("models/cube.obj");

	unsigned char *texture;
	int width, height, n;

	texture = stbi_load("textures/paper_with_holes.png", &width, &height, &n, 0);

	cube.loadTexture(texture, width, height, n);
}

// ---------------------------------------
void Reshape( int width, int height ) {
	glViewport( 0, 0, width, height );
	matProj = glm::perspective(glm::radians(80.0f), width/(float)height, 0.1f, 50.0f);
}

// ---------------------------------------------------
void Keyboard( unsigned char key, int x, int y ) {
    switch( key ) {
		case 27:	// ESC key
			exit(0);
			break;

		case 'w':
			cube.updateAngleX(0.1);
			break;

		case 's':
			cube.updateAngleX(-0.1);
			break;

		case 'd':
			cube.updateAngleY(0.1);
			break;

		case 'a':
			cube.updateAngleY(-0.1);
			break;
    }

    glutPostRedisplay();
}


// ---------------------------------------------------
int main( int argc, char *argv[] )
{
	// GLUT
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutInitWindowSize( 500, 500 );
	glutCreateWindow( "Obracanie klawiszami WSAD" );


	glutDisplayFunc( DisplayScene );
	glutReshapeFunc( Reshape );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutMouseWheelFunc( MouseWheel );
	glutKeyboardFunc( Keyboard );
	glutSpecialFunc( SpecialKeys );

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
