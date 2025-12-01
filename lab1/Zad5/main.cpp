#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <iostream>

#include "shader_stuff.h"
#include "house.h"

#define RMIN -1.0
#define RMAX 1.0


// ---------------------------------------
// Identyfikatory obiektow OpenGL

GLuint idProgram;	// programu
GLuint idVAO;		// tablic wierzcholkow
GLuint idVBO_coord; // bufora na wspolrzedne


class Triangle {
public:
    GLfloat px1, py1, px2, py2, px3, py3;

    Triangle(
        GLfloat _px1, GLfloat _py1,
        GLfloat _px2, GLfloat _py2,
        GLfloat _px3, GLfloat _py3):
    px1(_px1), py1(_py1), px2(_px2), py2(_py2), px3(_px3), py3(_py3) {}
};


int n = NUMBER_OF_TRIANGLES;
std::vector<Triangle> triangles;

// ---------------------------------------
// Wspolrzedne wierzchokow
/*GLfloat triangles[2*3*2] =
{
	-0.8f, -0.8f,
	 0.0f, -0.8f,
	-0.4f,  0.0f,

	 0.0f,  0.0f,
	 0.8f,  0.0f,
	 0.4f,  0.8f
};*/

// ---------------------------------------
void DisplayScene()
{
	// ---------------------------
	// Etap (5) rendering
	// ---------------------------
	glClear( GL_COLOR_BUFFER_BIT );


	// Wlaczenie potoku
	glUseProgram( idProgram );


		// Generowanie obiektow na ekranie
		glBindVertexArray( idVAO );
		glDrawArrays( GL_TRIANGLES, 0, NUMBER_OF_VERTICES );
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
	//glBufferData( GL_ARRAY_BUFFER, sizeof( triangles ) * n, &triangles, GL_STATIC_DRAW );
	glBufferData( GL_ARRAY_BUFFER, sizeof( triangles ) * n, &triangles.front(), GL_STATIC_DRAW );

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
int main( int argc, char *argv[] ) {

    for ( int i = 0; i < NUMBER_OF_TRIANGLES; i++ ) {
        int j = i * 6;

        triangles.push_back(Triangle(
            Mesh_Vertices[j], Mesh_Vertices[j + 1],
            Mesh_Vertices[j + 2], Mesh_Vertices[j + 3],
            Mesh_Vertices[j + 4], Mesh_Vertices[j + 5]));
    }

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
