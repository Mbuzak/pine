#include "scene.hpp"
#include "stb_image.h"

Scene::Scene() {
	d = display_init(800, 600, "pine");
	chess = new chschr::Chess();

	controller.mouse_button_left = 0;
	controller.mouse_button_right = 0;
}

void Scene::Setup() {
	stbi_set_flip_vertically_on_load(true);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	camera.init({0.0, -3.0, -22.0}, {0.3, -1.57});
	reshape(d.width, d.height);

	program_default = program_init("default");
	program_color = program_init("color");
	renderer_skybox.init();

	// Load models
	std::vector<std::string> model_names = {"square", "pawn", "knight", "bishop", "rook", "king", "queen", "chessboard", "sphere"};
	for (std::string &name: model_names) {
		Mesh* mesh = new Mesh();
		mesh_texture_init(mesh, name);
		meshes.insert({name, mesh});
	}
	
	// Load textures
	std::vector<std::string> texture_names = {"white", "black", "chessboard"};
	for (std::string &name: texture_names) {
		std::string file = name + ".jpg";
		textures.insert({name, texture_2d_init(file.c_str())});
	}

	// --- Shapes ---
	terrain = terrain_init();
	board = Shape(meshes.at("chessboard"), {0.0, 0.0, 0.0}, textures.at("chessboard"));

	// --- Lights ---
	sun = sun_init({1.0, -2.0, 2.0});

	lamps_[0] = new Lamp(meshes.at("sphere"), {9.0, 1.0, 9.0}, {5.2, 0.3, 0.5});
	lamps_[1] = new Lamp(meshes.at("sphere"), {9.0, 1.0, -9.0}, {0.4, 0.4, 0.6});
	lamps_[2] = new Lamp(meshes.at("sphere"), {-9.0, 1.0, 9.0}, {0.2, 0.9, 0.5});
	lamps_[3] = new Lamp(meshes.at("sphere"), {-9.0, 1.0, -9.0}, {0.2, 0.3, 0.5});

	dir_shadow_map.Init(sun.direction);
	fbo_init(&fbo);

	for (int i = 0; i < squares_.size(); i++) {
		squares_[i] = new Shape(meshes.at("square"), IndexToPosition(i));
		if (((i % 8) + (i / 8)) % 2 == 0) {
			squares_[i]->material_.ambient = glm::vec3(0.5);
		}
	}

	for (int i = 0; i < chess->mBoard.size(); i++) {
		std::string name = chess->pieceName.at(chess->mBoard[i]);
		std::string colour = (chess->isWhite(i / 8, i % 8)) ? "white" : "black";

		if (name == "x")
			continue;

		Piece *piece = new Piece(i, meshes.at(name), textures.at(colour));
		piece->colour = colour;
		if (colour == "white") {
			piece->rot.y = 3.2;
		}
		pieces_.push_back(piece);
	}

	printf("\n---Skróty klawiszowe---\n\n");
	printf("RPM - obrót sceny\n");
	printf("LPM - selecja obiektów\n");
	printf("Scroll - przybliżanie/oddalanie sceny\n\n");
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
		case SDL_KEYDOWN:
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
		__CHECK_FOR_ERRORS

		dir_shadow_map.Render(pieces_);

		glViewport(0, 0, d.width, d.height);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo.id);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderToTexture();

		glViewport(0, 0, d.width, d.height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		camera.Update();

		renderer_skybox.render(&camera);
		//RenderLights();
		RenderShapes(program_default);

		SDL_GL_SwapWindow(d.window);
	}
}

void Scene::RenderToTexture() {
	glUseProgram(program_default);
	uniform_mat4f_send(program_default, "matProj", camera.perspective);
	board.Display(program_default);
	glUseProgram(0);
}

void Scene::RenderShapes(GLuint program_id) {
	glUseProgram(program_id);

	// Send light
	for (int i = 0; i < lamps_.size(); i++) {
		std::string name = "lights[" + std::to_string(i) + "].";
		uniform_light_point_send(program_id, name, lamps_[i]);
	}
	uniform_light_directional_send(program_id, "sun.", &sun);
	uniform_mat4f_send(program_id, "matProj", camera.perspective);
	camera.SendUniform(program_id);

	// potok graficzny mapy cieni ?
	uniform_mat4f_send(program_id, "lightProj", dir_shadow_map.lightProj);
	uniform_mat4f_send(program_id, "lightView", dir_shadow_map.lightView);

	dir_shadow_map.SendTexture(program_id);

	// rysowanie obiektów nie-selekcyjnych (identyfikator 0)
	glStencilFunc(GL_ALWAYS, 0, 0xFF);

	render(program_id, &terrain);
	board.Display(program_id);
	
	glUseProgram(program_color);
	camera.SendUniform(program_color);
	uniform_mat4f_send(program_color, "matProj", camera.perspective);
	uniform_vec3f_send(program_color, "color", {0.2, 0.8, 0.2});
	for (int &value: active_fields) {
		squares_[value]->Display(program_color);
	}

	glUseProgram(program_id);
	for (int i = 0; i < pieces_.size(); ++i) {
		glStencilFunc(GL_ALWAYS, i + 1, 0xFF);
		pieces_[i]->Display(program_id);
	}

	if (selected_id >= 0) {
		glUseProgram(program_color);
		uniform_vec3f_send(program_color, "color", {0.0, 0.0, 0.35});
		pieces_[selected_id]->DisplayOutline(program_color, selected_id);
	}

	glUseProgram(0);
}

void Scene::RenderLights() {
	glUseProgram(program_color);

	for (int i = 0; i < 4; i++) {
		uniform_vec3f_send(program_default, "color", lamps_[i]->diffuse);
		lamps_[i]->Display(program_default);
	}
}

// game
std::vector<Piece*> Scene::get_pieces() {
	return pieces_;
}

void Scene::DisactivatePiece(Piece &piece) {
	if (piece.colour == "white") {
		if (off_rank_white < 8.0) {
			piece.pos.x = 12.0;
			piece.pos.z = off_rank_white;
		}
		else {
			piece.pos.x = 14.0;
			piece.pos.z = off_rank_white - 16.0;
		}
		
		off_rank_white += 2.0;
	}
	else {
		if (off_rank_black < 8.0) {
			piece.pos.x = -12.0;
			piece.pos.z = off_rank_black;
		}
		else {
			piece.pos.x = -14.0;
			piece.pos.z = off_rank_black - 16.0;
		}
		off_rank_black += 2.0;
	}

	piece.is_active = false;
}

glm::vec3 Scene::IndexToPosition(int id) {
	glm::vec3 position;

	position.x = ((id % 8) - 4) * 2.25 + 1.12;
	position.y = 0.15;
	position.z = ((id / 8) - 4) * 2.25 + 1.12;

	return position;
}

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
		std::string field = pieces_[selected_id]->get_field();
		//std::cout << "Field: " << field << "\n";
		int field_id = (field[0] - 'a') + 8 * ('8' - field[1]);
		std::cout << "Field id: " << field_id << "\n";

		for (int &value: chess->LegalMoves(field_id)) {
			//std::cout << value << "\n";
			active_fields.push_back(value);
		}

		std::cout << "\nPos: (" << pieces_[selected_id]->pos.x << ", " <<
			pieces_[selected_id]->pos.y << ", " <<
			pieces_[selected_id]->pos.z << ")\n";
	}
}

void Scene::move_piece() {
	if (selected_id < 0) {
		return;
	}

	std::string field = pieces_[selected_id]->get_field();
	
	int rank = 4 + (int)((pieces_[selected_id]->pos.z + 22.5) / 2.25) - 10;
	int file = 4 + (int)((pieces_[selected_id]->pos.x + 22.5) / 2.25) - 10;

	//std::cout << "rank: " << rank << ", file: " << file << "\n";
	//std::cout << (char)('a' + file) << (char)('8' - rank) << "\n";

	std::string new_field = std::string() + (char)('a' + file) + (char)('8' - rank);

	// new position
	if (field != new_field) {
		chschr::Move move((field + new_field).c_str());
		if (chess->perform(move)) {
			pieces_[selected_id]->update_position();

			std::string remove_field = pieces_[selected_id]->get_field();

			for (Piece *piece: pieces_) {
				if (piece->get_field() == remove_field && piece != pieces_[selected_id] && piece->is_active) {
					DisactivatePiece(*piece);
				}
			}
		}
		pieces_[selected_id]->update_world_position();
	}

	active_fields.clear();
	selected_id = -1;
}

void Scene::rotate(int x, int y) {
	camera.rot.y += 2 * (x - controller.mouse_pos.x) / (float)d.width;
	controller.mouse_pos.x = x;
	camera.rot.x -= 2 * (controller.mouse_pos.y - y) / (float)d.height;
	controller.mouse_pos.y = y;
}

void Scene::motion(int x, int y) {
	if (selected_id < 0) {
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, fbo.id);
	GLfloat depth;
	glReadPixels(x, d.height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::vec3 point = glm::unProject(glm::vec3(x, d.height - y, depth), camera.view, camera.perspective, glm::vec4(0, 0, d.width, d.height));
	//std::cout << "Worldspace: (" << point.x << ", " << point.y << ", " << point.z << "); Screen: (" << x << ", " << y << ")\n";

	// Update piece world position
	pieces_[selected_id]->pos.x = point.x;
	pieces_[selected_id]->pos.z = point.z;
}

void Scene::reshape(int w, int h) {
	d.width = w;
	d.height = h;

	glViewport(0, 0, d.width, d.height);
	camera.update_perspective(d.width / (float)d.height);
}
