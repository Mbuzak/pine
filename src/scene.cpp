#include "scene.hpp"
#include "stb_image.h"

void Scene::Setup() {
	d = display_init(1280, 720, "pine");
	controller_init(&controller);

	camera = { .pos = {-22.0, 9.0, -0.0}, .rot = {-15, 0} };
	camera_light = { .pos = {-10.0, 2.0, -10.0}, .rot = {-45, 60} };

	stbi_set_flip_vertically_on_load(true);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	sun = sun_init();

	shader_skybox_init(&shaders[SHADERS_SKYBOX]);
	shader_outline_init(&shaders[SHADERS_OUTLINE]);
	shader_rendered_init(&shaders[SHADERS_RENDERED]);
	shader_shadow_map_init(&shaders[SHADERS_SHADOW_MAP]);

	renderer_skybox_init(&renderer_skybox, &shaders[SHADERS_SKYBOX]);
	dir_shadow_map.Init(&shaders[SHADERS_SHADOW_MAP]);

	const mat4s proj = perspective_projection_compute(d.width, d.height);
	mat4s proj_light = orthographic_projection_compute();
	mat4s view_light = camera_view_compute(&camera_light);
	uniform_mat4_send(shaders, SHADERS_COUNT, UNIFORM_LIGHT_PROJECTION, proj_light);
	uniform_mat4_send(shaders, SHADERS_COUNT, UNIFORM_LIGHT_VIEW, view_light);
	uniform_mat4_send(shaders, SHADERS_COUNT, UNIFORM_PROJECTION, proj);

	// Load models
	const int MODEL_COUNT = 6;
	char model_names[MODEL_COUNT][16] = {"pawn", "knight", "bishop", "rook", "king", "queen"};
	for (int i = 0; i < MODEL_COUNT; i++) {
		Mesh mesh;
		mesh_texture_init(&mesh, model_names[i]);
		meshes.insert({model_names[i], mesh});
	}

	renderer_terrain_init(&renderer_terrain);

	// Load textures
	textures = new GLuint[2];
	textures[TEX_WHT] = texture_2d_init("white.jpg");
	textures[TEX_BLC] = texture_2d_init("black.jpg");

	for (int i = 0; i < 4; i++) {
		lamp_init(&lamps[i], {(rand() % 40) - 20, 0.2, (rand() % 40) - 20});
	}

	for (int i = 0; i < 32; i++) {
		std::string name = model_names[rand() % MODEL_COUNT];
		GLuint t = (rand() % 2) ? textures[TEX_WHT] : textures[TEX_BLC];
		vec3s pos = { (rand() % 40) - 20, 0.1, (rand() % 40) - 20 };
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
		vec3s dir = direction_compute(camera.rot);
		vec3s right = right_vector_compute(dir);
		const float speed = 0.5;
		if (controller.keys_pressed[SDLK_w] == 1) {
			vec3s mydir = glms_vec3_scale(dir, speed);
			camera.pos = glms_vec3_add(camera.pos, mydir);
		}
		if (controller.keys_pressed[SDLK_s] == 1) {
			vec3s mydir = glms_vec3_scale(dir, speed);
			camera.pos = glms_vec3_sub(camera.pos, mydir);
		}
		if (controller.keys_pressed[SDLK_a] == 1) {
			vec3s mydir = glms_vec3_scale(right, speed);
			camera.pos = glms_vec3_add(camera.pos, mydir);
		}
		if (controller.keys_pressed[SDLK_d] == 1) {
			vec3s mydir = glms_vec3_scale(right, speed);
			camera.pos = glms_vec3_sub(camera.pos, mydir);
		}
		__CHECK_FOR_ERRORS

		// Shadow FBO
		dir_shadow_map.Render(pieces);

		// World model detection FBO
		RenderToTexture(shaders[SHADERS_RENDERED].id);

		// Default FBO
		glViewport(0, 0, d.width, d.height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		mat4s view = camera_view_compute(&camera);
		uniform_vec3_send(shaders, SHADERS_COUNT, UNIFORM_CAMERA_COORDS, camera.pos);
		uniform_mat4_send(shaders, SHADERS_COUNT, UNIFORM_VIEW, view);

		renderer_skybox_render(&renderer_skybox);

		RenderShapes(shaders[SHADERS_RENDERED].id);

		SDL_GL_SwapWindow(d.window);
	}
}

void Scene::RenderToTexture(GLuint program_id) {
	glViewport(0, 0, d.width, d.height);
	glBindFramebuffer(GL_FRAMEBUFFER, renderer_terrain.frame.fbo_id);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program_id);
	renderer_terrain_render(&renderer_terrain, &shaders[SHADERS_RENDERED]);
	glUseProgram(0);
}

void Scene::RenderShapes(GLuint program_id) {
	// rysowanie obiektów nie-selekcyjnych (identyfikator 0)
	glStencilFunc(GL_ALWAYS, 0, 0xFF);

	glUseProgram(program_id);
	// Send light
	for (int i = 0; i < lamps.size(); i++) {
		std::string name = "lights[" + std::to_string(i) + "].";
		uniform_light_point_send(program_id, name.c_str(), &lamps[i].light);
		glUniform3fv(glGetUniformLocation(program_id, (name + "position").c_str()), 1, lamps[i].transform.pos.raw);
	}
	uniform_light_directional_send(program_id, "sun.", &sun);
	vec3s light_dir = direction_compute(camera_light.rot);
	glUniform3fv(glGetUniformLocation(program_id, "sun.direction"), 1, light_dir.raw);

	dir_shadow_map.SendTexture(program_id);

	glUniform1i(glGetUniformLocation(shaders[SHADERS_RENDERED].id, "is_terrain"), 1);
	renderer_terrain_render(&renderer_terrain, &shaders[SHADERS_RENDERED]);
	glUniform1i(glGetUniformLocation(shaders[SHADERS_RENDERED].id, "is_terrain"), 0);
	
	glUseProgram(program_id);
	for (int i = 0; i < pieces.size(); ++i) {
		glStencilFunc(GL_ALWAYS, i + 1, 0xFF);
		rendered_shape_render(&shaders[SHADERS_RENDERED], &pieces[i]);
	}

	if (selected_id >= 0) {
		shader_outline_render(&shaders[SHADERS_OUTLINE], selected_id,
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

	glBindFramebuffer(GL_FRAMEBUFFER, renderer_terrain.frame.fbo_id);
	GLfloat depth;
	glReadPixels(x, d.height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

	const float FOV = glm_rad(60.0);
	const float ASPECT_RATIO = d.width / (float)d.height;
	const float PLANE_NEAR = 0.1;
	const float PLANE_FAR = 200.0;
	const mat4s projection = glms_perspective(FOV, ASPECT_RATIO, PLANE_NEAR, PLANE_FAR);
	mat4s view = camera_view_compute(&camera);
	mat4s world_space = glms_mat4_mul(projection, view);
	const vec3s window_coords = {{(float)x, d.height - (float)y, depth}};
	const vec4s viewport = {{0, 0, (float)d.width, (float)d.height}};
	vec3s point = glms_unproject(window_coords, world_space, viewport);

	// Update piece world position
	pieces[selected_id].transform.pos.x = point.x;
	pieces[selected_id].transform.pos.z = point.z;
}

mat4s perspective_projection_compute(float width, float height) {
	const float FOV = glm_rad(60.0);
	const float ASPECT_RATIO = width / (float)height;
	const float PLANE_NEAR = 0.1;
	const float PLANE_FAR = 200.0;
	return glms_perspective(FOV, ASPECT_RATIO, PLANE_NEAR, PLANE_FAR);
}

mat4s orthographic_projection_compute() {
	const float SIZE = 18.0;
	const float PLANE_NEAR = 2.0;
	const float PLANE_FAR = 35.5;
	return glms_ortho(-SIZE, SIZE, -SIZE, SIZE, PLANE_NEAR, PLANE_FAR);
}

int selection_id_compute(int x, int y) {
	GLuint stencil;
	glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &stencil);
	return stencil - 1;
}
