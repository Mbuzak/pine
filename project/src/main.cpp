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

int Window_Width = 800;
int Window_Height = 600;

// Funkcja wywolywana przy nacisnieciu tzw. specjalnych
// klawiszy (klawiszy spoza tablicy ASCII)
inline void SpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		scene.camera_.pos.x -= 0.2f;
		break;
	case GLUT_KEY_RIGHT:
		scene.camera_.pos.x += 0.2f;
		break;
	case GLUT_KEY_UP:
		scene.camera_.pos.y += 0.2f;
		break;
	case GLUT_KEY_DOWN:
		scene.camera_.pos.y -= 0.2f;
		break;
	}

  glutPostRedisplay();
}

// Funkcja wywolywana podczas ruchu rolki myszy
inline void mouseWheel(int button, int dir, int x, int y) {
	if (dir > 0) {
		// Zoom in
		scene.camera_.pos.z += 0.5f;
	} else {
		// Zoom out
		scene.camera_.pos.z -= 0.5f;
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

	// ------------------------------------------------------------
	// Ustawienie domyslnego odsuniecia kamery od polozenia (0,0,0)
	// ------------------------------------------------------------
	scene.camera_.pos = {0.0, -3.0, -22.0};
	scene.camera_.rot = {0.3, -1.57};

	stbi_set_flip_vertically_on_load(true);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	scene.Setup();
}

void Reshape(int width, int height) {
	Window_Width = width;
	Window_Height = height;

	glViewport(0, 0, width, height);
	scene.camera_.perspective = glm::perspectiveFov(glm::radians(60.0f), (float)width, (float)height, 0.1f, 200.f);
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
			GLbyte color[4];
			//GLfloat depth;
			GLuint stencil;

			glReadPixels(x, Window_Height - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
			glReadPixels(x, Window_Height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
			glReadPixels(x, Window_Height - y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &stencil);

			printf("\nColor: %d %d %d\n", (unsigned char)color[0], (unsigned char)color[1], (unsigned char)color[2]);
			printf("Depth: %f\n", depth);
			printf("Stencil: %d\n", stencil);

			scene.selected_id = stencil - 1;

			if (scene.selected_id >= 0) {
				if (scene.get_pieces()[scene.selected_id]->is_active == false) {
					scene.selected_id = -1;
				}
			}

			if (scene.selected_id >= 0) {
				std::string field = scene.get_pieces()[scene.selected_id]->get_field();
				//std::cout << "Field: " << field << "\n";
				int field_id = (field[0] - 'a') + 8 * ('8' - field[1]);
				std::cout << "Field id: " << field_id << "\n";

				for (int &value: scene.chess->LegalMoves(field_id)) {
					//std::cout << value << "\n";
					scene.active_fields.push_back(value);
				}

				std::cout << "\nPos: (" << scene.get_pieces()[scene.selected_id]->position_.x << ", " <<
					scene.get_pieces()[scene.selected_id]->position_.y << ", " <<
					scene.get_pieces()[scene.selected_id]->position_.z << ")\n";
			}
		}
		else if (state == GLUT_UP) {
			if (scene.selected_id >= 0) {
				std::string field = scene.get_pieces()[scene.selected_id]->get_field();
				
				int rank, file;

				rank = 4 + (int)((scene.get_pieces()[scene.selected_id]->position_.z + 22.5) / 2.25) - 10;
				file = 4 + (int)((scene.get_pieces()[scene.selected_id]->position_.x + 22.5) / 2.25) - 10;

				//std::cout << "rank: " << rank << ", file: " << file << "\n";
				//std::cout << (char)('a' + file) << (char)('8' - rank) << "\n";

				std::string new_field = std::string() + (char)('a' + file) + (char)('8' - rank);
				

				// new position
				if (field != new_field) {
					chschr::Move move((field + new_field).c_str());
					if (scene.chess->perform(move)) {
						scene.get_pieces()[scene.selected_id]->update_position();

						std::string remove_field = scene.get_pieces()[scene.selected_id]->get_field();

						for (Piece *piece: scene.get_pieces()) {
							if (piece->get_field() == remove_field && piece != scene.get_pieces()[scene.selected_id] && piece->is_active) {

								scene.DisactivatePiece(*piece);
							}
						}

						scene.get_pieces()[scene.selected_id]->update_world_position();
					}
					else {
						scene.get_pieces()[scene.selected_id]->update_world_position();
					}
				}

				scene.active_fields.clear();
				scene.selected_id = -1;
			}
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
		scene.camera_.pos.z += 0.5;
	}
	else if (button == 4) {
		scene.camera_.pos.z -= 0.5;
	}

	glutPostRedisplay();
}

void MouseMotion(int x, int y) {
	if (scene._mouse_buttonState == GLUT_DOWN) {
		scene.camera_.rot.y += 2*(x - scene._mouse_buttonX)/(float)Window_Width;
		scene._mouse_buttonX = x;
		scene.camera_.rot.x -= 2*(scene._mouse_buttonY - y)/(float)Window_Height;
		scene._mouse_buttonY = y;
		glutPostRedisplay();
	}

	if (scene._mouse_left_click_state == GLUT_DOWN) {
		if (scene.selected_id >= 0) {

			glBindFramebuffer(GL_FRAMEBUFFER, scene.fbo.id);
			glReadPixels(x, Window_Height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glm::vec3 point = glm::unProject(glm::vec3(x, Window_Height - y, depth), scene.camera_.view, scene.camera_.perspective, glm::vec4(0, 0, Window_Width, Window_Height));
			//std::cout << "Worldspace: (" << point.x << ", " << point.y << ", " << point.z << "); Screen: (" << x << ", " << y << ")\n";

			//scene.get_pieces()[scene.selected_id]->position_.x = point.x;
			//scene.get_pieces()[scene.selected_id]->position_.z = point.z;

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
	glutInitWindowSize(Window_Width, Window_Height);
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
