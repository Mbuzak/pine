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

inline bool _select;

inline glm::mat4 _projection_matrix;
inline glm::mat4 _view_matrix;

// --------------------------------------------------------------
// Funkcja zwraca macierz widoku dla kamery
// --------------------------------------------------------------
inline glm::mat4 UpdateViewMatrix() {
	glm::mat4 matView = glm::mat4x4(1.0);
	matView = glm::translate(matView, glm::vec3(CameraTranslate_x, CameraTranslate_y, CameraTranslate_z));
	matView = glm::rotate(matView, CameraRotate_x, glm::vec3(1.0f, 0.0f, 0.0f));
	matView = glm::rotate(matView, CameraRotate_y, glm::vec3(0.0f, 1.0f, 0.0f));

	return matView;
}


// --------------------------------------------------------------
// NOWE: funkcja wyliczajaca pozycje kamery z macierzy widoku
// zrodlo: https://community.khronos.org/t/extracting-camera-position-from-a-modelview-matrix/68031
// --------------------------------------------------------------
inline glm::vec3 ExtractCameraPos(const glm::mat4 & a_modelView) {
  // Get the 3 basis vector planes at the camera origin and transform them into model space.
  //
  // NOTE: Planes have to be transformed by the inverse transpose of a matrix
  //       Nice reference here: http://www.opengl.org/discussion_boards/showthread.php/159564-Clever-way-to-transform-plane-by-matrix
  //
  //       So for a transform to model space we need to do:
  //            inverse(transpose(inverse(MV)))
  //       This equals : transpose(MV) - see Lemma 5 in http://mathrefresher.blogspot.com.au/2007/06/transpose-of-matrix.html
  //
  // As each plane is simply (1,0,0,0), (0,1,0,0), (0,0,1,0) we can pull the data directly from the transpose matrix.
  //
  glm::mat4 modelViewT = transpose(a_modelView);

  // Get plane normals
  glm::vec3 n1(modelViewT[0]);
  glm::vec3 n2(modelViewT[1]);
  glm::vec3 n3(modelViewT[2]);

  // Get plane distances
  float d1(modelViewT[0].w);
  float d2(modelViewT[1].w);
  float d3(modelViewT[2].w);

  // Get the intersection of these 3 planes
  // http://paulbourke.net/geometry/3planes/
  glm::vec3 n2n3 = cross(n2, n3);
  glm::vec3 n3n1 = cross(n3, n1);
  glm::vec3 n1n2 = cross(n1, n2);

  glm::vec3 top = (n2n3 * d1) + (n3n1 * d2) + (n1n2 * d3);
  float denom = dot(n1, n2n3);

  return top / -denom;
}

// --------------------------------------------------------------
// Funkcja wywolywana przy nacisnieciu tzw. specjalnych
// klawiszy (klawiszy spoza tablicy ASCII)
// --------------------------------------------------------------
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
