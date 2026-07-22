#include "scene.hpp"
#include "stb_image.h"

/* Obligatory default global settings - 3D app
 * Mouse: ON
 * Directional light: ON
 * Point light: ON
 * Skybox: ON
 * Terrain: ON
 * Directional shadow: ON
 * Chess: ON
 */
int app_init(Scene* app, int window_width, int window_height, const char* window_name) {
	app->d = display_init(window_width, window_height, window_name);

	// Enable mouse
	app->controller.mouse_button_left = 0;
	app->controller.mouse_button_right = 0;
	for (int i = 0; i < 128; i++) {
		app->controller.keys_pressed[i] = 0;
	}

	// Camera settings
	app->camera.fov = 60.0f;
	app->camera.near_plane = 0.1f;
	app->camera.far_plane = 200.0f;

	camera_light_init(&app->camera_light, {-10.12, 2.0, -10.12}, {-43, 65});

	stbi_set_flip_vertically_on_load(true);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	renderer_skybox_init(&app->renderer_skybox);
	//app->terrain = terrain_init();

	app->sun = sun_init();
	app->dir_shadow_map.Init(&app->camera_light);
	frame_init(&app->frame);

	app->chess = new chschr::Chess();

	return 0;
}

/* Optional settings */
void Scene::Setup() {
	reshape(d.width, d.height);
	GLuint grass = texture_2d_init("grass.jpg");
	terrain = Shape(&meshes.at("square"), {0.0, -0.1, 0.0}, grass);
	terrain.transform.scale = 80;

	board = Shape(&meshes.at("chessboard"), {0.0, 0.0, 0.0}, textures[TEX_CHS]);

	// --- Lights ---
	lamp_init(&lamps[0], {9.0, 0.2, 9.0}, &meshes.at("sphere"));
	lamp_init(&lamps[1], {9.0, 0.2, -9.0}, &meshes.at("sphere"));
	lamp_init(&lamps[2], {-9.0, 0.2, 9.0}, &meshes.at("sphere"));
	lamp_init(&lamps[3], {-9.0, 0.2, -9.0}, &meshes.at("sphere"));

	for (int i = 0; i < squares_.size(); i++) {
		squares_[i] = Shape(&meshes.at("square"), IndexToPosition(i));
	}

	for (int i = 0; i < chess->mBoard.size(); i++) {
		std::string name = chess->pieceName.at(chess->mBoard[i]);
		std::string colour = (chess->isWhite(i / 8, i % 8)) ? "white" : "black";

		if (name == "x")
			continue;

		GLuint t = (colour == "white") ? textures[TEX_WHT] : textures[TEX_BLC];
		Piece *piece = new Piece(i, &meshes.at(name), t);
		piece->colour = colour;
		if (colour == "white") {
			piece->shape.transform.rot.y = 180;
		}
		pieces_.push_back(piece);
	}

	printf("\nESC - exit\n");
	printf("LPM/RPM - select/rotate\n");
	printf("Scroll - zoom\n\n");
}

int Scene::events_handle() {
	SDL_Event e;
	int quit = 0;
	while (SDL_PollEvent(&e) != 0) {
		switch (e.type) {
			case SDL_QUIT:
				quit = 1;
				break;

			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
					reshape(e.window.data1, e.window.data2);
				}
				break;

			case SDL_KEYUP:
				controller.keys_pressed[e.key.keysym.sym] = 0;
				break;

			case SDL_KEYDOWN:
				controller.keys_pressed[e.key.keysym.sym] = 1;
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					quit = 1;
				}

				break;

			case SDL_MOUSEMOTION:
				if (controller.mouse_button_right == 1) {
					rotate(e.motion.x, e.motion.y);
				}

				if (controller.mouse_button_left == 1) {
					motion(e.motion.x, e.motion.y);
				}
				break;

			case SDL_MOUSEWHEEL:
				camera.pos.z += 0.5 * e.wheel.y;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (e.button.button == SDL_BUTTON_LEFT) {
					controller.mouse_button_left = 1;
					select_piece(d.width, d.height, e.button.x, e.button.y);
				}
				if (e.button.button == SDL_BUTTON_RIGHT) {
					controller.mouse_button_right = 1;
					controller.mouse_pos.x = e.button.x;
					controller.mouse_pos.y = e.button.y;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (e.button.button == SDL_BUTTON_LEFT) {
					controller.mouse_button_left = 0;
					move_piece();
				}

				if (e.button.button == SDL_BUTTON_RIGHT) {
					controller.mouse_button_right = 0;
				}
				break;

			default:
				break;
		}
	}

	return quit;
}

void Scene::display() {
	while (events_handle() != 1) {
		glm::vec3 dir = camera_dir_compute(camera.rot);
		glm::vec3 right = camera_right_compute(dir);
		glm::vec3 speed = glm::vec3(0.5);
		if (controller.keys_pressed[SDLK_w] == 1) {
			glm::vec3 mydir = speed * dir;
			camera.pos += mydir;
		}
		if (controller.keys_pressed[SDLK_s] == 1) {
			glm::vec3 mydir = speed * dir;
			camera.pos -= mydir;
		}
		if (controller.keys_pressed[SDLK_a] == 1) {
			glm::vec3 mydir = speed * right;
			camera.pos += mydir;
		}
		if (controller.keys_pressed[SDLK_d] == 1) {
			glm::vec3 mydir = speed * right;
			camera.pos -= mydir;
		}
		__CHECK_FOR_ERRORS

		// Shadow FBO
		dir_shadow_map.Render(&camera_light, pieces_);

		// World model detection FBO
		RenderToTexture(program_default);

		// Default FBO
		glViewport(0, 0, d.width, d.height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		camera.view = view_matrix_compute(camera.pos, camera.rot);

		renderer_skybox_render(&renderer_skybox, &camera);
		lamps_render(lamps, program_color);
		RenderShapes(program_default);

		SDL_GL_SwapWindow(d.window);
	}
}

void Scene::RenderToTexture(GLuint program_id) {
	glViewport(0, 0, d.width, d.height);
	glBindFramebuffer(GL_FRAMEBUFFER, frame.fbo_id);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program_id);
	shader_static_projection_send(&shader_static, camera.projection);
	shape_render(program_id, &board);
	glUseProgram(0);
}

void Scene::RenderShapes(GLuint program_id) {
	glUseProgram(program_id);

	// Send light
	for (int i = 0; i < lamps.size(); i++) {
		std::string name = "lights[" + std::to_string(i) + "].";
		uniform_light_point_send(program_id, name, &lamps[i].light);
		uniform_vec3f_send(program_id, (name + "position").c_str(), lamps[i].transform.pos);
	}
	uniform_light_directional_send(program_id, "sun.", &sun);
	glm::vec3 light_dir = camera_dir_compute(camera_light.rot);
	uniform_vec3f_send(program_id, "sun.direction", light_dir);
	shader_static_projection_send(&shader_static, camera.projection);
	camera_send_uniform(program_id, camera.pos, camera.rot);

	// potok graficzny mapy cieni ?
	uniform_mat4f_send(program_id, "lightProj", camera_light.projection);
	uniform_mat4f_send(program_id, "lightView", camera_light.view);

	dir_shadow_map.SendTexture(program_id);

	// rysowanie obiektów nie-selekcyjnych (identyfikator 0)
	glStencilFunc(GL_ALWAYS, 0, 0xFF);

	shape_render(program_id, &terrain);
	shape_render(program_id, &board);
	
	glUseProgram(program_color);
	camera_send_uniform(program_color, camera.pos, camera.rot);
	shader_static_projection_send(&shader_static, camera.projection);
	uniform_vec3f_send(program_color, "color", {0.2, 0.8, 0.2});
	for (int &value: active_fields) {
		shape_render(program_color, &squares_[value]);
	}

	glUseProgram(program_id);
	for (int i = 0; i < pieces_.size(); ++i) {
		glStencilFunc(GL_ALWAYS, i + 1, 0xFF);
		if (pieces_[i]->is_active) {
			shape_render(program_id, &pieces_[i]->shape);
		}
	}

	if (selected_id >= 0) {
		glUseProgram(program_color);
		uniform_vec3f_send(program_color, "color", {0.0, 0.0, 0.35});
		Shape shape = pieces_[selected_id]->shape;
		outline_render(program_color, selected_id, &shape.transform, shape.mesh);
	}

	glUseProgram(0);
}


// game
void Scene::select_piece(int wx, int wy, int x, int y) {
	GLbyte color[4];
	GLfloat depth;
	GLuint stencil;

	glReadPixels(x, wy - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
	glReadPixels(x, wy - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	glReadPixels(x, wy - y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &stencil);

	printf("\nColor: %d %d %d\n", (unsigned char)color[0], (unsigned char)color[1], (unsigned char)color[2]);
	printf("Depth: %f\n", depth);
	printf("Stencil: %d\n", stencil);

	selected_id = stencil - 1;

	if (selected_id >= 0) {
		if (pieces_[selected_id]->is_active == false) {
			selected_id = -1;
		}
	}

	if (selected_id >= 0) {
		std::string field = pieces_[selected_id]->field;
		//std::cout << "Field: " << field << "\n";
		int field_id = (field[0] - 'a') + 8 * ('8' - field[1]);
		std::cout << "Field id: " << field_id << "\n";

		for (int &value: chess->LegalMoves(field_id)) {
			//std::cout << value << "\n";
			active_fields.push_back(value);
		}

		std::cout << "\nPos: (" << pieces_[selected_id]->shape.transform.pos.x << ", " << pieces_[selected_id]->shape.transform.pos.y << ", " << pieces_[selected_id]->shape.transform.pos.z << ")\n";
	}
}

void Scene::move_piece() {
	if (selected_id < 0) {
		return;
	}

	std::string field = pieces_[selected_id]->field;
	std::string new_field = square_compute(pieces_[selected_id]->shape.transform.pos);

	// new position
	if (field != new_field) {
		chschr::Move move((field + new_field).c_str());
		if (chess->perform(move)) {
			for (Piece *piece: pieces_) {
				if (piece->field == new_field && piece->is_active) {
					piece->is_active = false;
				}
			}
			pieces_[selected_id]->field = new_field;
		}
		pieces_[selected_id]->update_world_position();
	}

	active_fields.clear();
	selected_id = -1;
}

void Scene::rotate(int x, int y) {
	const float rad_to_degree = 57.3;
	camera.rot.y += 2 * rad_to_degree * (x - controller.mouse_pos.x) / (float)d.width;
	controller.mouse_pos.x = x;
	camera.rot.x -= 2 * rad_to_degree * (controller.mouse_pos.y - y) / (float)d.height;
	controller.mouse_pos.y = y;
}

void Scene::motion(int x, int y) {
	if (selected_id < 0) {
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, frame.fbo_id);
	GLfloat depth;
	glReadPixels(x, d.height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::vec3 point = glm::unProject(glm::vec3(x, d.height - y, depth), camera.view, camera.projection, glm::vec4(0, 0, d.width, d.height));
	//std::cout << "Worldspace: (" << point.x << ", " << point.y << ", " << point.z << "); Screen: (" << x << ", " << y << ")\n";

	// Update piece world position
	pieces_[selected_id]->shape.transform.pos.x = point.x;
	pieces_[selected_id]->shape.transform.pos.z = point.z;
}

void Scene::reshape(int w, int h) {
	d.width = w;
	d.height = h;

	glViewport(0, 0, d.width, d.height);
	camera.projection = projection_matrix_compute(&camera,
		d.width / (float)d.height);
}

glm::vec3 IndexToPosition(int id) {
	glm::vec3 pos = {
		((id % 8) - 4) * 2.25 + 1.12,
		0.15,
		((id / 8) - 4) * 2.25 + 1.12
	};

	return pos;
}

void lamps_render(std::array<Lamp, 4> lamps, GLuint program_id) {
	glUseProgram(program_id);

	for (int i = 0; i < 4; i++) {
		uniform_vec3f_send(program_id, "color", lamps[i].light.diffuse);
		solid_render(program_id, &lamps[i].transform, lamps[i].mesh);
	}
}
