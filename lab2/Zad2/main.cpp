#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>

#include "shader_stuff.h"


// ---------------------------------------
// Identyfikatory obiektow OpenGL

GLuint idProgram;	// programu
GLuint idProgram2;

GLuint idVAO;		// tablic wierzcholkow
GLuint idVBO_coord; // bufora na wspolrzedne


class Triangle {
public:
    GLfloat px1, py1, px2, py2, px3, py3;

    Triangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3):
    mX1(x1), mY1(y1), mX2(x2), mY2(y2), mX3(x3), mY3(y3) {}

private:
	GLfloat mX1, mY1, mX2, mY2, mX3, mY3;
};


// ---------------------------------------
// Wspolrzedne wierzchokow

int n = 100;
std::vector<Triangle> triangles;
std::vector<Triangle> triangles2;

GLfloat getRandomNumber(float, float);

Triangle getTriangle();



// ---------------------------------------
void DisplayScene() {
	// ---------------------------
	// Etap (5) rendering
	// ---------------------------
	glClear( GL_COLOR_BUFFER_BIT );

	// Wlaczenie potoku
	glUseProgram( idProgram );

    // Generowanie obiektow na ekranie
	glBindVertexArray( idVAO );

	glDrawArrays( GL_TRIANGLES, 0, n / 2 * 3 );

	// Wlaczenie potoku
	glUseProgram( idProgram2 );

	// Generowanie obiektow na ekranie
	glDrawArrays( GL_TRIANGLES, n / 2 * 3, n / 2 * 3 );
	
	// Wylaczanie
	glBindVertexArray( 0 );
	glUseProgram( 0 );

	glutSwapBuffers();
}

// ---------------------------------------
void Initialize() {
	// -------------------------------------------------
	// Etap (2) przeslanie danych wierzcholk�w do OpenGL
	// -------------------------------------------------
	glGenVertexArrays( 1, &idVAO );
	glBindVertexArray( idVAO );

	// Bufor na wspolrzedne wierzcholkow
	glGenBuffers( 1, &idVBO_coord );
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_coord );
	glBufferData( GL_ARRAY_BUFFER, sizeof( triangles ) * n, &triangles.front(), GL_STATIC_DRAW );

	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );

	glBindVertexArray( 0 );

	// ---------------------------------------
	// Etap (3) stworzenie potoku graficznego
	// ---------------------------------------
	idProgram = glCreateProgram();
	idProgram2 = glCreateProgram();

	glAttachShader( idProgram, LoadShader(GL_VERTEX_SHADER, "vertex_grey.glsl"));
	glAttachShader( idProgram, LoadShader(GL_FRAGMENT_SHADER, "fragment_grey.glsl"));
	glAttachShader( idProgram2, LoadShader(GL_VERTEX_SHADER, "vertex_color.glsl"));
	glAttachShader( idProgram2, LoadShader(GL_FRAGMENT_SHADER, "fragment_color.glsl"));

	LinkAndValidateProgram( idProgram );
	LinkAndValidateProgram( idProgram2 );

	// -----------------------------------------
	// Etap (4) ustawienie maszyny stanow OpenGL
	// -----------------------------------------

	// ustawienie koloru czyszczenia ramki koloru
	glClearColor( 0.9f, 0.9f, 0.9f, 0.9f );

}

// ---------------------------------------
// Funkcja wywolywana podczas zmiany rozdzielczosci ekranu
void Reshape( int width, int height ) {
	glViewport( 0, 0, width, height );
}



// ---------------------------------------------------
// Funkcja wywolywana podczas wcisniecia klawisza ASCII
void Keyboard( unsigned char key, int x, int y ) {
    switch(key)
    {
		case 27:	// ESC key
			// opuszczamy glowna petle
			glutLeaveMainLoop();
			break;

		case ' ':	// Space key
			printf("SPACE!\n");
			// ponowne renderowanie
			glutPostRedisplay();
			break;
    }
}


// ---------------------------------------------------
int main( int argc, char *argv[] ) {
    srand(time(NULL));

	for ( int i = 0; i < n; i++ )
		triangles.push_back(getTriangle());
	
	// -----------------------------------------------
	// Etap (1) utworzynie kontektu i okna aplikacji
	// -----------------------------------------------

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

	// Inicjalizacja
	Initialize();



	glutMainLoop();


	// Cleaning
	glDeleteProgram( idProgram );
	glDeleteProgram( idProgram2 );
	glDeleteVertexArrays( 1, &idVBO_coord );
	glDeleteVertexArrays( 1, &idVAO );

	return 0;
}


GLfloat getRandomNumber(float min = -1.0, float max = 1.0) {
	return (GLfloat)min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}

Triangle getTriangle() {
	return Triangle(
		getRandomNumber(), getRandomNumber(),
		getRandomNumber(), getRandomNumber(),
		getRandomNumber(), getRandomNumber()
	);
}
