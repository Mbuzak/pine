#include <iostream>
#include <map>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utilities.hpp"
#include "obj_loader.hpp"
#include "scene.hpp"
#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define ANIMATION_OFF 0
#define ANIMATION_ON 1

#define DIRECTION_OFF 0
#define DIRECTION_ON 1

// Macierze PVM
glm::mat4 matProj;
Scene scene;
float Time = 0.0;
int dir = 1;
int dir2 = 1;
int mod = 0;
bool animation = ANIMATION_OFF;

void DisplayScene() {
	// Czyszczenie bufora koloru i glebokosci
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	__CHECK_FOR_ERRORS

	scene.display(matProj);

	glutSwapBuffers();
}

void Initialize() {
	// Ustawienia globalne
	glEnable(GL_DEPTH_TEST);
	glClearColor( 0.3f, 0.3f, 0.3f, 1.0f );

	// ------------------------------------------------------------
	// Ustawienie domyslnego odsuniecia kamery od polozenia (0,0,0)
	// ------------------------------------------------------------
	CameraTranslate_x = 0.0;
	CameraTranslate_y = -1.0;
	CameraTranslate_z = -12.0;
	CameraRotate_x = 0.4;

	stbi_set_flip_vertically_on_load(true);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	scene.setup();
	
	unsigned char *texture;
	int n;
	int w, h;

	texture = stbi_load("textures/wood.jpg", &w, &h, &n, 0);

	scene.mModels.at("Cubes").loadTexture(0, texture, w, h, n);
}

void Reshape( int width, int height ) {
	glViewport( 0, 0, width, height );
	matProj = glm::perspective(glm::radians(80.0f), width/(float)height, 0.1f, 50.0f);
}

void Keyboard( unsigned char key, int x, int y ) {
    switch( key ) {
	case 27:	// ESC key
		glutLeaveMainLoop();
		break;
	case '1':
		scene.reflectionLight = 0;
		break;
	case '2':
		scene.reflectionLight = 1;
		break;
	case '3':
		scene.light = 0;
		break;
	case '4':
		scene.light = 1;
		break;
	case '5':
		animation = ANIMATION_OFF;
		break;
	case '6':
		animation = ANIMATION_ON;
		break;
	case '7':
		scene.direction = DIRECTION_OFF;
		break;
	case '8':
		scene.direction = DIRECTION_ON;
		break;
    }
}

float xyz = 0.0;

void Animation( int frame ) {
	Time += 0.01;

	if ( scene.mModels.at("Cubes").mMeshes[0].mPosition.x > 7.0 ) {
		dir *= -1;
	}
	else if ( scene.mModels.at("Cubes").mMeshes[0].mPosition.x < 3.0 ) {
		dir *= -1;
		mod = (mod + 1) % 3;
	}

	scene.mModels.at("Cubes").mMeshes[0].mPosition.x += 0.02 * dir;
	scene.mModels.at("Cubes").mMeshes[1].mPosition.x -= 0.02 * dir;
	scene.mModels.at("Cubes").mMeshes[2].mPosition.z += 0.02 * dir;
	scene.mModels.at("Cubes").mMeshes[3].mPosition.z -= 0.02 * dir;

	if ( mod == 0 ) {
		scene.mModels.at("Cubes").mMeshes[0].mAngle.x += 0.01;
		scene.mModels.at("Cubes").mMeshes[1].mAngle.x -= 0.01;
		scene.mModels.at("Cubes").mMeshes[2].mAngle.z += 0.01;
		scene.mModels.at("Cubes").mMeshes[3].mAngle.z -= 0.01;
	}
	else if ( mod == 1 ) {
		scene.mModels.at("Cubes").mMeshes[0].mAngle.y += 0.02;
		scene.mModels.at("Cubes").mMeshes[1].mAngle.y -= 0.02;
		scene.mModels.at("Cubes").mMeshes[2].mAngle.x += 0.02;
		scene.mModels.at("Cubes").mMeshes[3].mAngle.x -= 0.02;
	}
	else {
		scene.mModels.at("Cubes").mMeshes[0].mAngle.z += 0.03;
		scene.mModels.at("Cubes").mMeshes[1].mAngle.z -= 0.03;
		scene.mModels.at("Cubes").mMeshes[2].mAngle.y += 0.03;
		scene.mModels.at("Cubes").mMeshes[3].mAngle.y -= 0.03;
	}

	if ( animation == ANIMATION_ON ) {
		if ( scene.mLightModel.mMeshes[0].Position.x > 10.0 ) {
			dir2 *= -1;
		}
		else if ( scene.mLightModel.mMeshes[0].Position.x < -10.0 ) {
			dir2 *= -1;
		}

		scene.mLightModel.mMeshes[0].Position.x += 0.05 * dir2;
		scene.mLightModel.mMeshes[0].Position.x += 0.05 * dir2;
	}

	glutPostRedisplay();
	glutTimerFunc(1000/60, Animation, 0);
}

int main( int argc, char *argv[] ) {
	srand( time( NULL ) );

	// GLUT
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutInitWindowSize( 600, 500 );
	glutCreateWindow( "Programownie grafiki 3D w OpenGL" );

	glutDisplayFunc( DisplayScene );
	glutReshapeFunc( Reshape );

	// linux way of handling mouse wheel events
	glutMouseFunc( MouseButton );

	glutMotionFunc( MouseMotion );

	// windows way of handling mouse wheel events
	glutMouseWheelFunc( mouseWheel );
	
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

	glutTimerFunc(1000/60, Animation, 0);
	glutMainLoop();

	// Kod wykonywany po wyjsciu z glownej petli

	// Cleaning

	return 0;
}
