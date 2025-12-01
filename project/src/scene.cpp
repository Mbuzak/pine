#include "scene.hpp"

Scene::Scene() {
	skybox_ = new Skybox();
	dir_shadow_map_ = new ShadowMap();

	game_ = new Game();

	fbo = new Framebuffer();
}

void Scene::Load(const char *folder) {
	for (const auto &entry: std::filesystem::directory_iterator(folder)) {
		std::string path = entry.path();
		std::string file = path.substr(path.find_last_of("/") + 1);
		std::string::size_type const p(file.find_last_of("."));

		std::string filename = file.substr(0, p);
		std::string extension = file.substr(p + 1, file.size());

		if (folder == "textures") AddTexture(filename.c_str(), extension.c_str());
		else if (folder == "models") AddModel(filename.c_str());
		else std::cout << "Folder " << folder << " isn't handled!\n";
	}
}

void Scene::Setup() {
	program_default = program_init("default");
	sbp = program_init("skybox");

	Load("models");
	Load("textures");

	// --- Shapes ---
	background_.push_back(new Shape(models_.at("ground"), glm::vec3{0.0, -0.1, 0.0}, textures_.at("grass")));
	background_.push_back(new Shape(models_.at("chessboard"), glm::vec3{0.0, 0.0, 0.0}, textures_.at("chessboard")));

	// --- Lights ---
	sun_ = new Sun(glm::vec3(1.0, -2.0, 2.0));

	lamps_[0] = new Lamp(models_.at("sphere"), glm::vec3(9.0, 1.0, 9.0), glm::vec3(5.2, 0.3, 0.5));
	lamps_[1] = new Lamp(models_.at("sphere"), glm::vec3(9.0, 1.0, -9.0), glm::vec3(0.4, 0.4, 0.6));
	lamps_[2] = new Lamp(models_.at("sphere"), glm::vec3(-9.0, 1.0, 9.0), glm::vec3(0.2, 0.9, 0.5));
	lamps_[3] = new Lamp(models_.at("sphere"), glm::vec3(-9.0, 1.0, -9.0), glm::vec3(0.2, 0.3, 0.5));

	skybox_->CreateVAO();

	dir_shadow_map_->Init(sun_->direction);

	game_->Init();

	fbo->init();

	printf("\n---Skróty klawiszowe---\n\n");
	printf("RPM - obrót sceny\n");
	printf("LPM - selecja obiektów\n");
	printf("Scroll - przybliżanie/oddalanie sceny\n\n");
}

void Scene::AddModel(const char *name) {
	models_.insert(std::pair<std::string, Model*>(name, new Model(name)));
}

void Scene::AddTexture(const char *filename, const char *extension) {
	std::string file = std::string(filename) + "." + std::string(extension);
	textures_.insert(std::pair<std::string, Texture*>(filename, new Texture(file.c_str())));
}

void Scene::Display() {
	RenderShadowMapOfDirectionalLight();


	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->id);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderToTexture();


	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	camera_.Update();

	RenderSkybox();
	//RenderLights();

	RenderShapes();

	glUseProgram(0);
}

void Scene::RenderToTexture() {
	glUseProgram(program_default);

	glUniformMatrix4fv(glGetUniformLocation(program_default, "matProj"), 1, GL_FALSE, glm::value_ptr(_projection_matrix));
	glUniform1i(glGetUniformLocation(program_default, "isLight"), false);

	background_[1]->Display(program_default);

	glUseProgram(0);
}

void Scene::SendLight() {
	GLint idProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &idProgram);

	// point lights
	for (int i = 0; i < lamps_.size(); i++) {
		std::string ambient = "lights[" + std::to_string(i) + "].ambient";
		std::string diffuse = "lights[" + std::to_string(i) + "].diffuse";
		std::string specular = "lights[" + std::to_string(i) + "].specular";
		std::string attenuation = "lights[" + std::to_string(i) + "].attenuation";
		std::string position = "lights[" + std::to_string(i) + "].position";

		glUniform3fv(glGetUniformLocation(idProgram, ambient.c_str()), 1, glm::value_ptr(lamps_[i]->ambient));
		glUniform3fv(glGetUniformLocation(idProgram, diffuse.c_str()), 1, glm::value_ptr(lamps_[i]->diffuse));
		glUniform3fv(glGetUniformLocation(idProgram, specular.c_str()), 1, glm::value_ptr(lamps_[i]->specular));
		glUniform3fv(glGetUniformLocation(idProgram, attenuation.c_str()), 1, glm::value_ptr(lamps_[i]->attenuation));
		glUniform3fv(glGetUniformLocation(idProgram, position.c_str()), 1, glm::value_ptr(lamps_[i]->position));
	}
}

void Scene::RenderShadowMapOfDirectionalLight() {
	dir_shadow_map_->Render(game_->get_pieces());
}

void Scene::RenderSkybox() {
	glUseProgram(sbp);

	skybox_->Draw(sbp, _projection_matrix, _view_matrix);
}

void Scene::RenderShapes() {
	glUseProgram(program_default);

	SendLight();
	sun_->SendUniform(program_default);

	glUniformMatrix4fv(glGetUniformLocation(program_default, "matProj"), 1, GL_FALSE, glm::value_ptr(_projection_matrix));
	glUniform1i(glGetUniformLocation(program_default, "isLight"), false);

	// send camera
	camera_.SendUniform(program_default);

	// potok graficzny mapy cieni ?
	glUniformMatrix4fv(glGetUniformLocation(program_default, "lightProj"), 1, GL_FALSE, glm::value_ptr(lightProj));
	glUniformMatrix4fv(glGetUniformLocation(program_default, "lightView"), 1, GL_FALSE, glm::value_ptr(lightView));

	dir_shadow_map_->SendTexture(program_default);

	// rysowanie obiektów nie-selekcyjnych (identyfikator 0)
	glStencilFunc(GL_ALWAYS, 0, 0xFF);

	for (Shape *shape : background_)
		shape->Display(program_default);
	
	game_->Display(program_default);
}

void Scene::RenderLights() {
	glUseProgram(program_default);
	glUniform1i(glGetUniformLocation(program_default, "isLight"), true);

	for (int i = 0; i < 4; i++) {
		glUniform3fv(glGetUniformLocation( program_default, "lightModelColor" ), 1, &lamps_[i]->diffuse[0]);
		lamps_[i]->Display(program_default);
	}
}
