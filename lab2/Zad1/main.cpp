#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <iostream>

#include "shader_stuff.h"


class Point {
public:
    Point(GLfloat x, GLfloat y):
    mX(x), mY(y) {}

private:
	GLfloat mX, mY;
};


// ---------------------------------------
// Identyfikatory obiektow OpenGL

GLuint idProgram;	// programu
GLuint idVAO;		// tablic wierzcholkow
GLuint idVBO_coord; // bufora na wspolrzedne


// ---------------------------------------
// Wspolrzedne wierzchokow
int n = 100;
std::vector<Point> points;

GLfloat getRandomNumber(float, float);

Point getPoint();


void DisplayScene() {
	glClear( GL_COLOR_BUFFER_BIT );

	glUseProgram( idProgram );
		glBindVertexArray( idVAO );
		glDrawArrays( GL_POINTS, 0, n );
		glBindVertexArray( 0 );
	glUseProgram( 0 );

	glutSwapBuffers();
}

void Initialize() {
	glGenVertexArrays( 1, &idVAO );
	glBindVertexArray( idVAO );

	glGenBuffers( 1, &idVBO_coord );
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_coord );
	glBufferData( GL_ARRAY_BUFFER, sizeof( points ) * n, &points.front(), GL_STATIC_DRAW );

	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );
	glBindVertexArray( 0 );

	idProgram = glCreateProgram();
	glAttachShader( idProgram, LoadShader(GL_VERTEX_SHADER, "vertex.glsl"));
	glAttachShader( idProgram, LoadShader(GL_FRAGMENT_SHADER, "fragment.glsl"));
	LinkAndValidateProgram( idProgram );

	glEnable(GL_PROGRAM_POINT_SIZE);
	glClearColor( 0.9f, 0.9f, 0.9f, 0.9f );
}

void Reshape( int width, int height ) {
	glViewport( 0, 0, width, height );
}

// Funkcja wywolywana podczas wcisniecia klawisza ASCII
void Keyboard( unsigned char key, int x, int y ) {
    switch(key) {
		case 27:	// ESC key
			glutLeaveMainLoop();
			break;
		case ' ':	// Space key
			printf("SPACE!\n");
			// ponowne renderowanie
			glutPostRedisplay();
			break;
    }
}


int main( int argc, char *argv[] ) {
    srand(time(NULL));
	
	for ( int i = 0; i < n; i++ )
        points.push_back(getPoint());

	// GLUT
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutInitWindowSize( 500, 500 );
	glutCreateWindow( "Szablon programu w OpenGL" );
	glutDisplayFunc( DisplayScene );
	glutReshapeFunc( Reshape );
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

	// Cleaning
	glDeleteProgram( idProgram );
	glDeleteVertexArrays( 1, &idVBO_coord );
	glDeleteVertexArrays( 1, &idVAO );

	return 0;
}

GLfloat getRandomNumber(float min = -1.0, float max = 1.0) {
	return (GLfloat)min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}

Point getPoint() {
	return Point(getRandomNumber(), getRandomNumber());
}
