#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>

#include "shader_stuff.h"


// ---------------------------------------
// Identyfikatory obiektow OpenGL

GLuint idProgram;	// programu
GLuint idProgram2;

GLuint idVAO;
GLuint idVAO2;		// tablic wierzcholkow
GLuint idVBO_coord; // bufora na wspolrzedne
GLuint idVBO_coord2;

// ---------------------------------------
// Wspolrzedne wierzchokow

class Triangle {
public:
    GLfloat x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8;

    Triangle(float width) {
        float diagonal = width * sqrt(2) / 2;

        x1 = 0.0;
        y1 = width;
        x2 = diagonal;
        y2 = diagonal;
        x3 = width;
        y3 = 0.0;
        x4 = diagonal;
        y4 = -1 * diagonal;

        x5 = 0.0;
        y5 = -1 * width;
        x6 = -1 * diagonal;
        y6 = -1 * diagonal;
        x7 = -1 * width;
        y7 = 0.0;
        x8 = -1 * diagonal;
        y8 = diagonal;
    }
};

int n = 1;
std::vector<Triangle> triangles;
std::vector<Triangle> border;
//float width = 0.7;

/*GLfloat triangles[8*2] = {
    0.0, width,
    diagonal, diagonal,
    width, 0.0,
    diagonal, -1 * diagonal,

    0.0, -1 * width,
    -1 * diagonal, -1 * diagonal,
    -1 * width, 0.0,
    -1 * diagonal, diagonal
};

GLfloat triangles2[8*2] = {
    0.0, width,
    diagonal, diagonal,
    width, 0.0,
    diagonal, -1 * diagonal,

    0.0, -1 * width,
    -1 * diagonal, -1 * diagonal,
    -1 * width, 0.0,
    -1 * diagonal, diagonal
};*/

// ---------------------------------------
void DisplayScene()
{
	// ---------------------------
	// Etap (5) rendering
	// ---------------------------
	glClear( GL_COLOR_BUFFER_BIT );

	// Wlaczenie potoku
	glUseProgram( idProgram2 );


		// Generowanie obiektow na ekranie
		glBindVertexArray( idVAO2 );
		//glDrawArrays( GL_TRIANGLE_STRIP_ADJACENCY, 0, 100*n );
		//glDrawArrays( GL_TRIANGLES, 0, 8*n );
		//glDrawArrays( GL_TRIANGLE_FAN, 0, 8 * n );
		glDrawArrays( GL_TRIANGLE_FAN, 0, 8*n );
		//glDrawArrays( GL_LINES, 0, 8*2 );
		glBindVertexArray( 0 );


    // Wlaczenie potoku
	glUseProgram( idProgram );


		// Generowanie obiektow na ekranie
		glBindVertexArray( idVAO );
		//glDrawArrays( GL_POLYGON, 0, 8*2 );
		//glDrawArrays( GL_TRIANGLES, 0, 8*2 );
		glDrawArrays( GL_TRIANGLE_FAN, 0, 8 * n );
		//glDrawArrays( GL_LINES, 0, 8*n );
		//glDrawArrays( GL_LINES, 0, 8*2 );
		glBindVertexArray( 0 );






	// Wylaczanie
	glUseProgram( 0 );


	glutSwapBuffers();
}

// ---------------------------------------
void Initialize()
{
	// -------------------------------------------------
	// Etap (2) przeslanie danych wierzcholków do OpenGL
	// -------------------------------------------------
	glGenVertexArrays( 1, &idVAO );
	glBindVertexArray( idVAO );

	// Bufor na wspolrzedne wierzcholkow
	glGenBuffers( 1, &idVBO_coord );
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_coord );
	glBufferData( GL_ARRAY_BUFFER, sizeof( triangles ) * 3 * n, &triangles.front(), GL_STATIC_DRAW );

	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );

	glBindVertexArray( 0 );


	glGenVertexArrays( 1, &idVAO2 );
	glBindVertexArray( idVAO2 );

	// Bufor na wspolrzedne wierzcholkow
	glGenBuffers( 1, &idVBO_coord2 );
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_coord2 );
	glBufferData( GL_ARRAY_BUFFER, sizeof( border ) * 3 * n, &border.front(), GL_STATIC_DRAW );

	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );

	glBindVertexArray( 0 );


	// ---------------------------------------
	// Etap (3) stworzenie potoku graficznego
	// ---------------------------------------
	idProgram = glCreateProgram();
	glAttachShader( idProgram, LoadShader(GL_VERTEX_SHADER, "vertex.glsl"));
	glAttachShader( idProgram, LoadShader(GL_FRAGMENT_SHADER, "fragment.glsl"));
	LinkAndValidateProgram( idProgram );

	idProgram2 = glCreateProgram();
	glAttachShader( idProgram2, LoadShader(GL_VERTEX_SHADER, "vertex_border.glsl"));
	glAttachShader( idProgram2, LoadShader(GL_FRAGMENT_SHADER, "fragment_border.glsl"));
	LinkAndValidateProgram( idProgram2 );

	// -----------------------------------------
	// Etap (4) ustawienie maszyny stanow OpenGL
	// -----------------------------------------

	// ustawienie koloru czyszczenia ramki koloru
	glClearColor( 0.9f, 0.9f, 0.9f, 0.9f );

}

// ---------------------------------------
// Funkcja wywolywana podczas zmiany rozdzielczosci ekranu
void Reshape( int width, int height )
{
	glViewport( 0, 0, width, height );
}



// ---------------------------------------------------
// Funkcja wywolywana podczas wcisniecia klawisza ASCII
void Keyboard( unsigned char key, int x, int y )
{
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
int main( int argc, char *argv[] )
{
	// -----------------------------------------------
	// Etap (1) utworzynie kontektu i okna aplikacji
	// -----------------------------------------------
	triangles.push_back(Triangle(0.7));
	border.push_back(Triangle(0.75));

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
	if( GLEW_OK != err )
	{
		printf("GLEW Error\n");
		exit(1);
	}

	// OpenGL
	if( !GLEW_VERSION_3_2 )
	{
		printf("Brak OpenGL 3.2!\n");
		exit(1);
	}

	// Inicjalizacja
	Initialize();



	glutMainLoop();


	// Cleaning
	glDeleteProgram( idProgram );
	glDeleteVertexArrays( 1, &idVBO_coord );
	glDeleteVertexArrays( 1, &idVAO );

	return 0;
}
