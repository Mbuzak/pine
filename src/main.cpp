#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "scene.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Scene scene;

void DisplayScene() {
	scene.Display();
}

void Initialize() {
	scene.Setup();
}

void Reshape(int width, int height) {
	scene.reshape(width, height);
}

void Animation(int frame) {
	glutPostRedisplay();
	glutTimerFunc(1000/60, Animation, 0);
}

void MouseButton(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		scene._mouse_left_click_state = state;

		if (state == GLUT_DOWN)	{
			scene.select_piece(scene.width, scene.height, x, y);
		}
		else if (state == GLUT_UP) {
			scene.move_piece();
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		scene._mouse_buttonState = state;

		if(state == GLUT_DOWN) {
			scene._mouse_buttonX = x;
			scene._mouse_buttonY = y;
		}
	}
	else if (button == 3) {
		scene.camera.pos.z += 0.5;
	}
	else if (button == 4) {
		scene.camera.pos.z -= 0.5;
	}

	glutPostRedisplay();
}

void MouseMotion(int x, int y) {
	if (scene._mouse_buttonState == GLUT_DOWN) {
		scene.rotate(x, y);
	}

	if (scene._mouse_left_click_state == GLUT_DOWN) {
		scene.motion(x, y);
	}
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowSize(scene.width, scene.height);
	glutCreateWindow("Chess 3D");

	glutDisplayFunc(DisplayScene);
	glutReshapeFunc(Reshape);

	// linux way of handling mouse wheel events
	glutMouseFunc(MouseButton);

	glutMotionFunc(MouseMotion);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		printf("GLEW Error\n");
		exit(1);
	}

	if (!GLEW_VERSION_3_2) {
		printf("Error: Not sufficient OpenGL version. Required at least 3.2");
		exit(1);
	}

	Initialize();

	glutTimerFunc(1000/60, Animation, 0);
	glutMainLoop();

	// Kod wykonywany po wyjsciu z glownej petli

	// Cleaning

	return 0;
}
