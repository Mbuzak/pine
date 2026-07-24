#include "scene.hpp"
#include "stb_image.h"

void Scene::Setup() {
	d = display_init(1280, 720, "pine");
	controller_init(&controller);

	camera = { .pos = {-22.0, 9.0, -0.0}, .rot = {-15, 0} };
	camera_light = { .pos = {-10.12, 2.0, -10.12}, .rot = {-43, 65} };

	stbi_set_flip_vertically_on_load(true);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	sun = sun_init();
	frame_init(&frame);

	// Shaders & renderers
	glm::mat4 proj_light = orthographic_projection_compute();
	glm::mat4 view_light = camera_view_compute(&camera_light);
	glm::mat4 projection = perspective_projection_compute(d.width, d.height);
	shader_rendered_init(&shader_rendered, projection);
	shader_outline_init(&shader_outline, projection);
	renderer_skybox_init(&renderer_skybox, projection);
	dir_shadow_map.Init(proj_light, view_light);

	// Load models
	std::vector<std::string> model_names = {"square", "pawn", "knight", "bishop", "rook", "king", "queen"};
	for (std::string &name: model_names) {
		Mesh mesh;
		mesh_texture_init(&mesh, name);
		meshes.insert({name, mesh});
	}

	// Load textures
	textures = new GLuint[2];
	textures[TEX_WHT] = texture_2d_init("white.jpg");
	textures[TEX_BLC] = texture_2d_init("black.jpg");

	GLuint grass = texture_2d_init("grass.png");
	shape_init(&terrain, {0, 0, 0}, &meshes.at("square"), grass);
	terrain.transform.scale = 80;

	for (int i = 0; i < 4; i++) {
		lamp_init(&lamps[i], {(rand() % 40) - 20, 0.2, (rand() % 40) - 20});
	}

	std::vector<std::string> cn = {"pawn", "knight", "bishop", "rook", "queen", "king"};
	for (int i = 0; i < 32; i++) {
		std::string name = cn[rand() % cn.size()];
		GLuint t = (rand() % 2) ? textures[TEX_WHT] : textures[TEX_BLC];
		glm::vec3 pos = { (rand() % 40) - 20, 0.1, (rand() % 40) - 20 };
		Shape piece;
		shape_init(&piece, pos, &meshes.at(name), t);
		pieces.push_back(piece);
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
					display_reshape(&d, e.window.data1, e.window.data2);
					// TODO: Send perspective projection
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
					selected_id = selection_id_compute(e.button.x, d.height - e.button.y);
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
					selected_id = -1;
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
		glm::vec3 dir = direction_compute(camera.rot);
		glm::vec3 right = right_vector_compute(dir);
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
		dir_shadow_map.Render(pieces);

		// World model detection FBO
		RenderToTexture(shader_rendered.id);

		// Default FBO
		glViewport(0, 0, d.width, d.height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glm::mat4 view = camera_view_compute(&camera);
		renderer_skybox_render(&renderer_skybox, view);
		RenderShapes(shader_rendered.id);

		SDL_GL_SwapWindow(d.window);
	}
}

void Scene::RenderToTexture(GLuint program_id) {
	glViewport(0, 0, d.width, d.height);
	glBindFramebuffer(GL_FRAMEBUFFER, frame.fbo_id);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program_id);
	rendered_shape_render(&shader_rendered, &terrain);
	glUseProgram(0);
}

void Scene::RenderShapes(GLuint program_id) {
	glm::mat4 view = camera_view_compute(&camera);
	glm::mat4 proj_light = orthographic_projection_compute();
	glm::mat4 view_light = camera_view_compute(&camera_light);

	glUseProgram(program_id);
	// Send light
	for (int i = 0; i < lamps.size(); i++) {
		std::string name = "lights[" + std::to_string(i) + "].";
		uniform_light_point_send(program_id, name, &lamps[i].light);
		uniform_vec3f_send(program_id, (name + "position").c_str(), lamps[i].transform.pos);
	}
	uniform_light_directional_send(program_id, "sun.", &sun);
	glm::vec3 light_dir = direction_compute(camera_light.rot);
	uniform_vec3f_send(program_id, "sun.direction", light_dir);
	shader_rendered_view_send(&shader_rendered, view);
	shader_rendered_camera_position_send(&shader_rendered, camera.pos);

	uniform_mat4f_send(program_id, "lightProj", proj_light);
	uniform_mat4f_send(program_id, "lightView", view_light);

	dir_shadow_map.SendTexture(program_id);

	// rysowanie obiektów nie-selekcyjnych (identyfikator 0)
	glStencilFunc(GL_ALWAYS, 0, 0xFF);

	glUniform1i(glGetUniformLocation(shader_rendered.id, "is_terrain"), 1);
	rendered_shape_render(&shader_rendered, &terrain);
	glUniform1i(glGetUniformLocation(shader_rendered.id, "is_terrain"), 0);
	
	glUseProgram(program_id);
	for (int i = 0; i < pieces.size(); ++i) {
		glStencilFunc(GL_ALWAYS, i + 1, 0xFF);
		rendered_shape_render(&shader_rendered, &pieces[i]);
	}

	if (selected_id >= 0) {
		shader_outline_render(&shader_outline, view, selected_id,
			&pieces[selected_id].transform, pieces[selected_id].mesh);
	}

	glUseProgram(0);
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

	glm::mat4 projection = perspective_projection_compute(d.width, d.height);
	glm::mat4 view = camera_view_compute(&camera);
	const glm::vec3 window_coords = {x, d.height - y, depth};
	const glm::vec4 viewport = {0, 0, d.width, d.height};
	glm::vec3 point = glm::unProject(window_coords, view, projection, viewport);

	// Update piece world position
	pieces[selected_id].transform.pos.x = point.x;
	pieces[selected_id].transform.pos.z = point.z;
}

glm::mat4 perspective_projection_compute(float width, float height) {
	const float FOV = glm::radians(60.0);
	const float ASPECT_RATIO = width / (float)height;
	const float PLANE_NEAR = 0.1;
	const float PLANE_FAR = 200.0;
	return glm::perspective(FOV, ASPECT_RATIO, PLANE_NEAR, PLANE_FAR);
}

glm::mat4 orthographic_projection_compute() {
	const float SIZE = 18.0;
	const float PLANE_NEAR = 2.0;
	const float PLANE_FAR = 35.5;
	return glm::ortho(-SIZE, SIZE, -SIZE, SIZE, PLANE_NEAR, PLANE_FAR);
}

int selection_id_compute(int x, int y) {
	GLuint stencil;
	glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &stencil);
	return stencil - 1;
}
