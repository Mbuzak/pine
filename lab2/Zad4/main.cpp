#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <math.h>

#include "shader_stuff.h"


// ---------------------------------------
// Identyfikatory obiektow OpenGL

GLuint idProgram;	// programu
GLuint idProgram2;

GLuint idVAO;		// tablic wierzcholkow
GLuint idVBO_coord; // bufora na wspolrzedne

// ---------------------------------------
// Wspolrzedne wierzchokow

class Point {
public:
	Point(GLfloat x, GLfloat y):
	mX(x), mY(y) {}

private:
	GLfloat mX, mY;
};


class Polygon {
public:
	Polygon() {}

    Polygon(int n, float r) {
		float alpha = (float)360 / n;  // inner angle
		GLfloat x1, y1, x2 = r, y2 = 0.0;
		float xc = 0.0, yc = 0.0;  // center point

		for ( int i = 1; i <= n; i++ ) {
			x1 = x2;
			y1 = y2;

			x2 = r * cos(toRadian(alpha * i)) + xc;
			y2 = r * sin(toRadian(alpha * i)) + yc;

			mPoints.push_back(Point(x1, y1));
		}
    }

	std::vector<Point> getPoints() {
		return mPoints;
	}

private:
	std::vector<Point> mPoints;

	float toRadian(float degree) {
		return degree * M_PI / 180;
	}
};

int n = 8;  // number of polygon vertices
float r = 0.7;  // polygon's radius
Polygon polygon;


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
	glDrawArrays( GL_TRIANGLE_FAN, 0, n );
	glBindVertexArray( 0 );

	// Wylaczanie
	glUseProgram( 0 );

	// Wlaczenie potoku
	glUseProgram( idProgram2 );
	// Generowanie obiektow na ekranie
	glBindVertexArray( idVAO );
	glDrawArrays( GL_LINE_LOOP, 0, n );
	glBindVertexArray( 0 );

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
	glBufferData( GL_ARRAY_BUFFER, sizeof( polygon ) * n, &polygon.getPoints().front(), GL_STATIC_DRAW );

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

	glLineWidth(10.0);

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
	polygon = Polygon(n, r);

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
	glDeleteVertexArrays( 1, &idVBO_coord );
	glDeleteVertexArrays( 1, &idVAO );
	glDeleteProgram( idProgram2 );

	return 0;
}
