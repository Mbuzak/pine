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

#include "utilities.hpp"
#include "scene.hpp"

#include "chess/chess.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float off_rank_white = -8.0;
float off_rank_black = -8.0;
Scene scene;
float Time = 0.0;

int Window_Width = 800;
int Window_Height = 600;


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
	CameraTranslate_x = 0.0;
	CameraTranslate_y = -3.0;
	CameraTranslate_z = -22.0;
	CameraRotate_x = 0.3;
	CameraRotate_y = -1.57;

	stbi_set_flip_vertically_on_load(true);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	scene.Setup();

	_select = false;
}

void Reshape(int width, int height) {
	Window_Width = width;
	Window_Height = height;

	glViewport(0, 0, width, height);
	_projection_matrix = glm::perspectiveFov(glm::radians(60.0f), (float)width, (float)height, 0.1f, 200.f);
}

void Animation(int frame) {
	Time += 0.01;

	glutPostRedisplay();
	glutTimerFunc(1000/60, Animation, 0);
}

GLfloat depth;

void MouseButton(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		_mouse_left_click_state = state;

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

			selected_id = stencil - 1;

			if (selected_id >= 0) {
				if (scene.game_->get_pieces()[selected_id]->is_active == false) {
					selected_id = -1;
				}
			}

			if (selected_id >= 0) {
				std::string field = scene.game_->get_pieces()[selected_id]->get_field();
				//std::cout << "Field: " << field << "\n";
				int field_id = (field[0] - 'a') + 8 * ('8' - field[1]);
				std::cout << "Field id: " << field_id << "\n";

				for (int &value: scene.game_->chess->LegalMoves(field_id)) {
					//std::cout << value << "\n";
					scene.game_->active_fields.push_back(value);
				}

				std::cout << "\nPos: (" << scene.game_->get_pieces()[selected_id]->position_.x << ", " <<
					scene.game_->get_pieces()[selected_id]->position_.y << ", " <<
					scene.game_->get_pieces()[selected_id]->position_.z << ")\n";
			}
		}
		else if (state == GLUT_UP) {
			if (selected_id >= 0) {
				std::string field = scene.game_->get_pieces()[selected_id]->get_field();
				
				int rank, file;

				rank = 4 + (int)((scene.game_->get_pieces()[selected_id]->position_.z + 22.5) / 2.25) - 10;
				file = 4 + (int)((scene.game_->get_pieces()[selected_id]->position_.x + 22.5) / 2.25) - 10;

				//std::cout << "rank: " << rank << ", file: " << file << "\n";
				//std::cout << (char)('a' + file) << (char)('8' - rank) << "\n";

				std::string new_field = std::string() + (char)('a' + file) + (char)('8' - rank);
				

				// new position
				if (field != new_field) {
					chschr::Move move((field + new_field).c_str());
					if (scene.game_->chess->perform(move)) {
						scene.game_->get_pieces()[selected_id]->update_position();

						std::string remove_field = scene.game_->get_pieces()[selected_id]->get_field();

						for (Piece *piece: scene.game_->get_pieces()) {
							if (piece->get_field() == remove_field && piece != scene.game_->get_pieces()[selected_id] && piece->is_active) {

								scene.game_->DisactivatePiece(*piece);
							}
						}

						scene.game_->get_pieces()[selected_id]->update_world_position();
					}
					else {
						scene.game_->get_pieces()[selected_id]->update_world_position();
					}
				}

				scene.game_->active_fields.clear();
				selected_id = -1;
			}
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		_mouse_buttonState = state;

		if(state == GLUT_DOWN) {
			_mouse_buttonX = x;
			_mouse_buttonY = y;
		}
	}
	else if (button == 3) {
		CameraTranslate_z += 0.5f;
	}
	else if (button == 4) {
		CameraTranslate_z -= 0.5f;
	}

	glutPostRedisplay();
}

void MouseMotion(int x, int y) {
	if (_mouse_buttonState == GLUT_DOWN) {
		CameraRotate_y += 2*(x - _mouse_buttonX)/(float)Window_Width;
		_mouse_buttonX = x;
		CameraRotate_x -= 2*(_mouse_buttonY - y)/(float)Window_Height;
		_mouse_buttonY = y;
		glutPostRedisplay();
	}

	if (_mouse_left_click_state == GLUT_DOWN) {
		if (selected_id >= 0) {

			glBindFramebuffer(GL_FRAMEBUFFER, scene.fbo->id);
			glReadPixels(x, Window_Height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glm::vec3 point = glm::unProject(glm::vec3(x, Window_Height - y, depth), _view_matrix, _projection_matrix, glm::vec4(0, 0, Window_Width, Window_Height));
			//std::cout << "Worldspace: (" << point.x << ", " << point.y << ", " << point.z << "); Screen: (" << x << ", " << y << ")\n";

			//scene.game_->get_pieces()[selected_id]->position_.x = point.x;
			//scene.game_->get_pieces()[selected_id]->position_.z = point.z;

			scene.game_->UpdatePieceWorldPosition(selected_id, point.x, point.z);
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
