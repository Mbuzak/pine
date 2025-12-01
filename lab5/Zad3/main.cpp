// ----------------------------------------
// Programowanie grafiki 3D w OpenGL
// ----------------------------------------
#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Lokalne pliki naglowkowe
#include "utilities.hpp"

// Biblioteka do obslugi plikow graficznych
# define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Macierze transformacji i rzutowania
glm::mat4 matProj = glm::mat4(1.0);
glm::mat4 matView = glm::mat4(1.0);
glm::mat4 matModel = glm::mat4(1.0);

// Identyfikatory obiektow
GLuint idProgram;
GLuint idVAO;
GLuint idVBO_coord;
GLuint idVBO_color;
GLuint idVBO_uv;

// Identyfikator obiektu tekstury
GLuint idTexture;


const int N = 50;

// Wspolrzedne dwoch kwadratow
GLfloat vertices[12*3*N] = {
    -1.000000, -1.000000, 0.000000,
    1.000000, 1.000000, 0.000000,
    1.000000, -1.000000, 0.000000,
    -1.000000, -1.000000, 0.000000,
    -1.000000, 1.000000, 0.000000,
    1.000000, 1.000000, 0.000000,

    0.000000, -1.000000, 1.000000,
    0.000000, 1.000000, -1.000000,
    0.000000, -1.000000, -1.000000,
    0.000000, -1.000000, 1.000000,
    0.000000, 1.000000, 1.000000,
    0.000000, 1.000000, -1.000000,
};

// Wspolrzedne UV dwoch kwadratow
GLfloat uvs[12*2*N] = {
	0.000000, 0.000000,
	0.98, 0.98,
	0.98, 0.000000,
	0.000000, 0.000000,
	0.000000, 0.98,
	0.98, 0.98,

	0.98, 0.000000,
	0.000000, 0.98,
	0.000000, 0.000000,
	0.98, 0.000000,
	0.98, 0.98,
	0.000000, 0.98,
};

// ---------------------------------------
void DisplayScene()
{
	// Czyszczenie bufora koloru i glebokosci
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Obliczanie macierzy widoku
	matView = UpdateViewMatrix();


	// Wlaczenie programu
	glUseProgram( idProgram );


		// Przekazanie macierzy
		glUniformMatrix4fv( glGetUniformLocation(idProgram, "matProj"), 1, GL_FALSE, glm::value_ptr(matProj));
		glUniformMatrix4fv( glGetUniformLocation(idProgram, "matView"), 1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv( glGetUniformLocation(idProgram, "matModel"), 1, GL_FALSE, glm::value_ptr(matModel));


		// Aktywowanie jednostki teksturujacej nr 0,
		// przypisanie do niej obiektu tekstury oraz
		// polaczenie j.t. z uchwytem tekstury w shaderze
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, idTexture);
		glUniform1i(glGetUniformLocation(idProgram, "tex"), 0);


		// Rendering
		glBindVertexArray( idVAO );
		glDrawArrays( GL_TRIANGLES, 0, 12 * N );
		glBindVertexArray( 0 );


	// Wylaczanie
	glUseProgram( 0 );


	glutSwapBuffers();

}

// ---------------------------------------
void Initialize()
{

	// Ustawienia maszyny OpenGL
	glEnable(GL_DEPTH_TEST);
	glClearColor( 0.7f, 0.7f, 0.7f, 1.0f );


	// Potok
	idProgram = glCreateProgram();
	glAttachShader( idProgram, LoadShader(GL_VERTEX_SHADER, "vertex.glsl"));
	glAttachShader( idProgram, LoadShader(GL_FRAGMENT_SHADER, "fragment.glsl"));
	LinkAndValidateProgram( idProgram );

	// copy 9x vertices and change x, z coordinates randomly
	for ( int k = 1; k <= N - 1; k++ ) {
		int rand_x = ( rand() % 2 == 0 ) ? rand() % 10 : -rand() % 10;
		int rand_z = ( rand() % 2 == 0 ) ? rand() % 10 : -rand() % 5;

		for ( int i = 0; i < 12 * 3; i++ ) {
			if ( i % 3 == 0 ) vertices[12 * 3 * k + i] = vertices[i] + rand_x;
			else if ( i % 3 == 1 ) vertices[12 * 3 * k + i] = vertices[i];
			else vertices[12 * 3 * k + i] = vertices[i] + rand_z;
		}
	}

	// copy 9x uvs
	for ( int k = 1; k <= N - 1; k++ ) {
		for ( int i = 0; i < 12 * 2; i++ ) {
			uvs[12 * 2 * k + i] = uvs[i];
		}
	}

	



	// VAO
	glGenVertexArrays( 1, &idVAO );
	glBindVertexArray( idVAO );

		// Bufor na wspolrzedne wierzcholkow
		glGenBuffers( 1, &idVBO_coord );
		glBindBuffer( GL_ARRAY_BUFFER, idVBO_coord );
		glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
		glEnableVertexAttribArray( 0 );

		// Bufor na wspolrzedne tekstury
		glGenBuffers( 1, &idVBO_uv );
		glBindBuffer( GL_ARRAY_BUFFER, idVBO_uv );
		glBufferData( GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW );
		glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, NULL );
		glEnableVertexAttribArray( 1 );

	glBindVertexArray( 0 );


	// Wczytanie pliku tekstury RGBA
	int tex_width, tex_height, tex_n;
	unsigned char *tex_data;

	// ustawienie odwracania wczytywania obrazow
	stbi_set_flip_vertically_on_load(true);

	// wczytanie pliku graficznego
	tex_data = stbi_load ("flower32bit.png", &tex_width , &tex_height , &tex_n, 0);
	if ( tex_data == NULL) {
		printf ("Image can�t be loaded!\n");
		exit(1);
	}

	// Utworzenie obiektu tekstury RGBA
	glGenTextures(1, &idTexture);
	glBindTexture(GL_TEXTURE_2D, idTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


}

// ---------------------------------------
void Reshape( int width, int height )
{
	glViewport( 0, 0, width, height );
	matProj = glm::perspective(glm::radians(80.0f), width/(float)height, 0.1f, 50.0f);
}

// --------------------------------------------------------------
void Keyboard( unsigned char key, int x, int y )
{
    switch(key)
    {
		case 27: // ESC key
			glutLeaveMainLoop();
			break;
    }

}

// ---------------------------------------------------
int main( int argc, char *argv[] )
{
	srand(time(NULL));

	// GLUT
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutInitWindowSize( 500, 500 );
	glutCreateWindow( "Programownie grafiki w OpenGL" );


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


	Initialize();


	glutMainLoop();


	// Cleaning
	glDeleteProgram( idProgram );
	glDeleteVertexArrays( 1, &idVBO_coord );
	glDeleteVertexArrays( 1, &idVBO_color );
	glDeleteVertexArrays( 1, &idVBO_uv );
	glDeleteVertexArrays( 1, &idVAO );
	glDeleteTextures(1, &idTexture );

	return 0;
}
