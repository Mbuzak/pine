#include "scene.hpp"
#include "stb_image.h"

void uniform_vec3f_send(GLuint program, const char* name, const glm::vec3& vec) {
	glUniform3fv(glGetUniformLocation(program, name), 1, glm::value_ptr(vec));
}

void uniform_mat4fv_send(GLuint program_id, const char* name, const glm::mat4& matrix) {
	glUniformMatrix4fv(glGetUniformLocation(program_id, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

Scene::Scene() {
	chess = new chschr::Chess();
}

void Scene::Setup() {
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

	program_default = program_init("default");
	renderer_skybox.init();

	// Load models
	std::vector<std::string> model_names = {"square", "pawn", "knight", "bishop", "rook", "king", "queen", "chessboard", "ground", "sphere"};
	for (std::string &name: model_names) {
		models_.insert({name, new Model(name)});
	}
	
	// Load textures
	std::vector<std::string> texture_names = {"white", "black", "chessboard", "grass"};
	for (std::string &name: texture_names) {
		std::string file = name + ".jpg";
		textures.insert({name, texture_2d_init(file.c_str())});
	}

	// Camera
	camera.pos = {0.0, -3.0, -22.0};
	camera.rot = {0.3, -1.57};

	// --- Shapes ---
	background_.push_back(new Shape(models_.at("ground"), glm::vec3{0.0, -0.1, 0.0}, textures.at("grass")));
	background_.push_back(new Shape(models_.at("chessboard"), glm::vec3{0.0, 0.0, 0.0}, textures.at("chessboard")));

	// --- Lights ---
	sun_ = new Sun(glm::vec3(1.0, -2.0, 2.0));

	lamps_[0] = new Lamp(models_.at("sphere"), glm::vec3(9.0, 1.0, 9.0), glm::vec3(5.2, 0.3, 0.5));
	lamps_[1] = new Lamp(models_.at("sphere"), glm::vec3(9.0, 1.0, -9.0), glm::vec3(0.4, 0.4, 0.6));
	lamps_[2] = new Lamp(models_.at("sphere"), glm::vec3(-9.0, 1.0, 9.0), glm::vec3(0.2, 0.9, 0.5));
	lamps_[3] = new Lamp(models_.at("sphere"), glm::vec3(-9.0, 1.0, -9.0), glm::vec3(0.2, 0.3, 0.5));

	dir_shadow_map.Init(sun_->direction);

	fbo.init();

	for (int i = 0; i < squares_.size(); i++) {
		squares_[i] = new Shape(models_.at("square"), IndexToPosition(i));
		if (((i % 8) + (i / 8)) % 2 == 0) {
			squares_[i]->material_.ambient = glm::vec3(0.5);
		}
	}

	for (int i = 0; i < chess->mBoard.size(); i++) {
		std::string name = chess->pieceName.at(chess->mBoard[i]);
		std::string colour = (chess->isWhite(i / 8, i % 8)) ? "white" : "black";

		if (name == "x")
			continue;

		Piece *piece = new Piece(i, models_.at(name), textures.at(colour));
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

void Scene::Display() {
	__CHECK_FOR_ERRORS

	dir_shadow_map.Render(get_pieces());

	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo.id);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderToTexture();

	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	camera.Update();

	renderer_skybox.render(&camera);
	//RenderLights();

	RenderShapes();

	glUseProgram(0);

	glutSwapBuffers();
}

void Scene::RenderToTexture() {
	glUseProgram(program_default);
	glUniformMatrix4fv(glGetUniformLocation(program_default, "matProj"), 1, GL_FALSE, glm::value_ptr(camera.perspective));
	glUniform1i(glGetUniformLocation(program_default, "isLight"), false);
	background_[1]->Display(program_default);
	glUseProgram(0);
}

void Scene::SendLight(GLuint program) {
	// point lights
	for (int i = 0; i < lamps_.size(); i++) {
		std::string name = "lights[" + std::to_string(i) + "].";

		uniform_vec3f_send(program, (name + "ambient").c_str(), lamps_[i]->ambient);
		uniform_vec3f_send(program, (name + "specular").c_str(), lamps_[i]->specular);
		uniform_vec3f_send(program, (name + "attenuation").c_str(), lamps_[i]->attenuation);
		uniform_vec3f_send(program, (name + "position").c_str(), lamps_[i]->position);
	}
}

void Scene::RenderShapes() {
	glUseProgram(program_default);
	SendLight(program_default);
	sun_->SendUniform(program_default);

	glUniformMatrix4fv(glGetUniformLocation(program_default, "matProj"), 1, GL_FALSE, glm::value_ptr(camera.perspective));
	glUniform1i(glGetUniformLocation(program_default, "isLight"), false);

	camera.SendUniform(program_default);

	// potok graficzny mapy cieni ?
	glUniformMatrix4fv(glGetUniformLocation(program_default, "lightProj"), 1, GL_FALSE, glm::value_ptr(lightProj));
	glUniformMatrix4fv(glGetUniformLocation(program_default, "lightView"), 1, GL_FALSE, glm::value_ptr(lightView));

	dir_shadow_map.SendTexture(program_default);

	// rysowanie obiektów nie-selekcyjnych (identyfikator 0)
	glStencilFunc(GL_ALWAYS, 0, 0xFF);

	for (Shape *shape : background_)
		shape->Display(program_default);
	
	Display(program_default);
}

void Scene::RenderLights() {
	glUseProgram(program_default);
	glUniform1i(glGetUniformLocation(program_default, "isLight"), true);

	for (int i = 0; i < 4; i++) {
		glUniform3fv(glGetUniformLocation( program_default, "lightModelColor" ), 1, &lamps_[i]->diffuse[0]);
		lamps_[i]->Display(program_default);
	}
}

// game
std::vector<Piece*> Scene::get_pieces() {
	return pieces_;
}

void Scene::Display(GLuint program_id) {
	for (int &value: active_fields) {
		//std::cout << value << "\n";
		glUniform1i(glGetUniformLocation(program_id, "active_field"), true);
		squares_[value]->Display(program_id);
		glUniform1i(glGetUniformLocation(program_id, "active_field"), false);
	}

	for (int i = 0; i < squares_.size(); i++) {
		squares_[i]->Display(program_id);
	}

	for (int i = 0; i < pieces_.size(); ++i) {
		glStencilFunc(GL_ALWAYS, i + 1, 0xFF);

		pieces_[i]->Display(program_id);
	}

	if (selected_id >= 0) {
		pieces_[selected_id]->DisplayOutline(program_id, selected_id);
	}
}

void Scene::UpdatePieceWorldPosition(int id, float x, float z) {
	get_pieces()[id]->pos.x = x;
	get_pieces()[id]->pos.z = z;
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
		if (get_pieces()[selected_id]->is_active == false) {
			selected_id = -1;
		}
	}

	if (selected_id >= 0) {
		std::string field = get_pieces()[selected_id]->get_field();
		//std::cout << "Field: " << field << "\n";
		int field_id = (field[0] - 'a') + 8 * ('8' - field[1]);
		std::cout << "Field id: " << field_id << "\n";

		for (int &value: chess->LegalMoves(field_id)) {
			//std::cout << value << "\n";
			active_fields.push_back(value);
		}

		std::cout << "\nPos: (" << get_pieces()[selected_id]->pos.x << ", " <<
			get_pieces()[selected_id]->pos.y << ", " <<
			get_pieces()[selected_id]->pos.z << ")\n";
	}
}

void Scene::move_piece() {
	if (selected_id < 0) {
		return;
	}

	std::string field = get_pieces()[selected_id]->get_field();
	
	int rank = 4 + (int)((get_pieces()[selected_id]->pos.z + 22.5) / 2.25) - 10;
	int file = 4 + (int)((get_pieces()[selected_id]->pos.x + 22.5) / 2.25) - 10;

	//std::cout << "rank: " << rank << ", file: " << file << "\n";
	//std::cout << (char)('a' + file) << (char)('8' - rank) << "\n";

	std::string new_field = std::string() + (char)('a' + file) + (char)('8' - rank);

	// new position
	if (field != new_field) {
		chschr::Move move((field + new_field).c_str());
		if (chess->perform(move)) {
			get_pieces()[selected_id]->update_position();

			std::string remove_field = get_pieces()[selected_id]->get_field();

			for (Piece *piece: get_pieces()) {
				if (piece->get_field() == remove_field && piece != get_pieces()[selected_id] && piece->is_active) {
					DisactivatePiece(*piece);
				}
			}
		}
		get_pieces()[selected_id]->update_world_position();
	}

	active_fields.clear();
	selected_id = -1;
}

void Scene::rotate(int x, int y) {
	camera.rot.y += 2*(x - _mouse_buttonX)/(float)width;
	_mouse_buttonX = x;
	camera.rot.x -= 2*(_mouse_buttonY - y)/(float)height;
	_mouse_buttonY = y;
	glutPostRedisplay();
}

void Scene::motion(int x, int y) {
	if (selected_id < 0) {
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, fbo.id);
	GLfloat depth;
	glReadPixels(x, height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::vec3 point = glm::unProject(glm::vec3(x, height - y, depth), camera.view, camera.perspective, glm::vec4(0, 0, width, height));
	//std::cout << "Worldspace: (" << point.x << ", " << point.y << ", " << point.z << "); Screen: (" << x << ", " << y << ")\n";

	UpdatePieceWorldPosition(selected_id, point.x, point.z);
}

void Scene::reshape(int w, int h) {
	width = w;
	height = h;

	glViewport(0, 0, width, height);
	camera.perspective = glm::perspectiveFov(glm::radians(60.0f), (float)width, (float)height, 0.1f, 200.f);
}
