#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <map>
#include <string>

#include "scene.hpp"

#include "chess/chess.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// NOWE: makro do znajdowania prostych bledow opengla
#define __CHECK_FOR_ERRORS 	{GLenum errCode; if ((errCode = glGetError()) != GL_NO_ERROR) printf("Error (%d): %s in file %s at line %d !\n", errCode, gluErrorString(errCode), __FILE__,  __LINE__);}

Scene scene;
float Time = 0.0;

// Funkcja wywolywana przy nacisnieciu tzw. specjalnych
// klawiszy (klawiszy spoza tablicy ASCII)
inline void SpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		scene.camera.pos.x -= 0.2f;
		break;
	case GLUT_KEY_RIGHT:
		scene.camera.pos.x += 0.2f;
		break;
	case GLUT_KEY_UP:
		scene.camera.pos.y += 0.2f;
		break;
	case GLUT_KEY_DOWN:
		scene.camera.pos.y -= 0.2f;
		break;
	}

  glutPostRedisplay();
}

// Funkcja wywolywana podczas ruchu rolki myszy
inline void mouseWheel(int button, int dir, int x, int y) {
	if (dir > 0) {
		scene.camera.pos.z += 0.5f;
	} else {
		scene.camera.pos.z -= 0.5f;
	}

  glutPostRedisplay();
}

void DisplayScene() {
	__CHECK_FOR_ERRORS

	scene.Display();

	glutSwapBuffers();
}

void Initialize() {
	// Ustawienia globalne
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	glEnable(GL_STENCIL_TEST);
	glClearStencil(0);

	/* Value of stencil buffer will be replaced only in case of
	positive pass stencil and depth test */
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	stbi_set_flip_vertically_on_load(true);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	scene.Setup();
}

void Reshape(int width, int height) {
	scene.width = width;
	scene.height = height;

	glViewport(0, 0, width, height);
	scene.camera.perspective = glm::perspectiveFov(glm::radians(60.0f), (float)width, (float)height, 0.1f, 200.f);
}

void Animation(int frame) {
	Time += 0.01;

	glutPostRedisplay();
	glutTimerFunc(1000/60, Animation, 0);
}

GLfloat depth;

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
		if (scene.selected_id >= 0) {
			glBindFramebuffer(GL_FRAMEBUFFER, scene.fbo.id);
			glReadPixels(x, scene.height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glm::vec3 point = glm::unProject(glm::vec3(x, scene.height - y, depth), scene.camera.view, scene.camera.perspective, glm::vec4(0, 0, scene.width, scene.height));
			//std::cout << "Worldspace: (" << point.x << ", " << point.y << ", " << point.z << "); Screen: (" << x << ", " << y << ")\n";

			scene.UpdatePieceWorldPosition(scene.selected_id, point.x, point.z);
		}
	}
}


int main(int argc, char *argv[]) {
	// srand(time(NULL));

	// GLUT
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

	// windows way of handling mouse wheel events
	glutMouseWheelFunc(mouseWheel);

	glutSpecialFunc(SpecialKeys);

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
