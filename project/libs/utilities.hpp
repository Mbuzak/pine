#ifndef __SHADER_STUFF
#define __SHADER_STUFF

// References:
// https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/loading.php

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// --------------------------------------------------------------
// NOWE: makro do znajdowania prostych bledow opengla
// --------------------------------------------------------------
#define __CHECK_FOR_ERRORS 	{GLenum errCode; if ((errCode = glGetError()) != GL_NO_ERROR) printf("Error (%d): %s in file %s at line %d !\n", errCode, gluErrorString(errCode), __FILE__,  __LINE__);}

// Zmienne kontrolujace polozenie i orientacje kamery
inline GLfloat CameraRotate_x = 0.0f;
inline GLfloat CameraRotate_y = 0.0f;
inline GLfloat CameraTranslate_x = 0.0f;
inline GLfloat CameraTranslate_y = 0.0f;
inline GLfloat CameraTranslate_z = -2.0f;

inline int selected_id = -1;

// Zmienne do kontroli stanu myszy
inline int _mouse_buttonState = GLUT_UP;
inline int _mouse_left_click_state = GLUT_UP;
inline int _mouse_buttonX;
inline int _mouse_buttonY;

// Funkcja zwraca macierz widoku dla kamery
inline glm::mat4 UpdateViewMatrix() {
	glm::mat4 matView = glm::mat4x4(1.0);
	matView = glm::translate(matView, glm::vec3(CameraTranslate_x, CameraTranslate_y, CameraTranslate_z));
	matView = glm::rotate(matView, CameraRotate_x, glm::vec3(1.0f, 0.0f, 0.0f));
	matView = glm::rotate(matView, CameraRotate_y, glm::vec3(0.0f, 1.0f, 0.0f));

	return matView;
}

// Funkcja wywolywana przy nacisnieciu tzw. specjalnych
// klawiszy (klawiszy spoza tablicy ASCII)
inline void SpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		CameraTranslate_x -= 0.2f;
		break;

	case GLUT_KEY_RIGHT:
		CameraTranslate_x += 0.2f;
		break;

	case GLUT_KEY_UP:
		CameraTranslate_y += 0.2f;
		break;

	case GLUT_KEY_DOWN:
		CameraTranslate_y -= 0.2f;
		break;
	}

  glutPostRedisplay();
}

// --------------------------------------------------------------
// Funkcja wywolywana podczas ruchu rolki myszy
// --------------------------------------------------------------
inline void mouseWheel(int button, int dir, int x, int y) {
	if (dir > 0) {
		// Zoom in
		CameraTranslate_z += 0.5f;
	} else {
		// Zoom out
		CameraTranslate_z -= 0.5f;
	}

  glutPostRedisplay();
}

#endif
