#include "stb_image.h"
#include "scene.hpp"

void Scene::Setup() {
	d = display_init(1280, 720, "pine");
	controller_init(&controller);

	camera = { .pos = {-22.0, 9.0, -0.0}, .rot = {-15, 0} };
	camera_light = { .pos = {-10.0, 2.0, -10.0}, .rot = {-45, 60} };

	stbi_set_flip_vertically_on_load(true);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	sun = sun_init();

	shader_outline_init(&shaders[SHADERS_OUTLINE]);
	shader_rendered_init(&shaders[SHADERS_RENDERED]);
	shader_shadow_map_init(&shaders[SHADERS_SHADOW_MAP]);
	shader_terrain_init(&shaders[SHADERS_TERRAIN]);

	shadow_map_init(&dir_shadow_map, &shaders[SHADERS_SHADOW_MAP]);

	const mat4s proj = perspective_projection_compute(d.width, d.height);
	mat4s proj_light = orthographic_projection_compute();
	mat4s view_light = camera_view_compute(&camera_light);
	uniform_mat4_send(shaders, SHADERS_COUNT, UNIFORM_LIGHT_PROJECTION, proj_light);
	uniform_mat4_send(shaders, SHADERS_COUNT, UNIFORM_LIGHT_VIEW, view_light);
	uniform_mat4_send(shaders, SHADERS_COUNT, UNIFORM_PROJECTION, proj);

	// Load models
	mesh_cube_init(&mesh_cube);

	renderer_terrain_init(&renderer_terrain, &shaders[SHADERS_TERRAIN]);

	for (int i = 0; i < 4; i++) {
		vec3s pos = {{(rand() % 40) - 20.0f, 0.2, (rand() % 40) - 20.0f}};
		lamp_init(&lamps[i], pos);
	}

	for (int i = 0; i < shape_count; i++) {
		vec3s pos = {{(rand() % 40) - 20.0f, 1.0, (rand() % 40) - 20.0f}};
		Shape shape;
		shape_init(&shape, pos, &mesh_cube);
		shapes[i] = shape;
	}

	vec3s light_dir = direction_compute(camera_light.rot);

	GLuint program_id;
	program_id = shaders[SHADERS_RENDERED].id;
	glUseProgram(program_id);
	for (int i = 0; i < lamps.size(); i++) {
		std::string name = "lights[" + std::to_string(i) + "].";
		uniform_light_point_send(program_id, name.c_str(), &lamps[i].light);
		glUniform3fv(glGetUniformLocation(program_id, (name + "position").c_str()), 1, lamps[i].transform.pos.raw);
	}
	uniform_light_directional_send(program_id, "sun.", &sun);
	glUniform3fv(glGetUniformLocation(program_id, "sun.direction"), 1, light_dir.raw);
	shadow_map_texture_send(&dir_shadow_map, program_id);

	program_id = shaders[SHADERS_TERRAIN].id;
	glUseProgram(program_id);
	for (int i = 0; i < lamps.size(); i++) {
		std::string name = "lights[" + std::to_string(i) + "].";
		uniform_light_point_send(program_id, name.c_str(), &lamps[i].light);
		glUniform3fv(glGetUniformLocation(program_id, (name + "position").c_str()), 1, lamps[i].transform.pos.raw);
	}
	uniform_light_directional_send(program_id, "sun.", &sun);
	glUniform3fv(glGetUniformLocation(program_id, "sun.direction"), 1, light_dir.raw);
	shadow_map_texture_send(&dir_shadow_map, program_id);
	glUseProgram(0);

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
					if (selected_id < 0) {
						break;
					}
					mat4s view = camera_view_compute(&camera);
					glBindFramebuffer(GL_FRAMEBUFFER, renderer_terrain.frame.fbo_id);
					vec3s world_coord = screen_to_world_space_convert(e.motion.x, e.motion.y, d.window, view);
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					shapes[selected_id].transform.pos.x = world_coord.x;
					shapes[selected_id].transform.pos.z = world_coord.z;
				}
				break;

			case SDL_MOUSEWHEEL:
				camera.pos.z += 0.5 * e.wheel.y;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (e.button.button == SDL_BUTTON_LEFT) {
					controller.mouse_button_left = 1;
					GLuint stencil;
					glReadPixels(e.button.x, d.height - e.button.y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &stencil);
					selected_id = stencil - 1;
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
		__CHECK_FOR_ERRORS
		camera_move(&camera, controller.keys_pressed, shaders, SHADERS_COUNT);

		shadow_map_render(&dir_shadow_map, shapes, shape_count);
		glViewport(0, 0, d.width, d.height);

		renderer_terrain_render(&renderer_terrain);

		glUseProgram(shaders[SHADERS_RENDERED].id);
		for (int i = 0; i < shape_count; ++i) {
			glStencilFunc(GL_ALWAYS, i + 1, 0xFF);
			rendered_shape_render(&shaders[SHADERS_RENDERED], &shapes[i]);
		}
		glStencilFunc(GL_ALWAYS, 0, 0xFF);

		shader_outline_render(&shaders[SHADERS_OUTLINE], selected_id,
			&shapes[selected_id].transform, shapes[selected_id].mesh);

		SDL_GL_SwapWindow(d.window);
	}
}

void Scene::rotate(int x, int y) {
	const float rad_to_degree = 57.3;
	camera.rot.y += 2 * rad_to_degree * (x - controller.mouse_pos.x) / (float)d.width;
	controller.mouse_pos.x = x;
	camera.rot.x -= 2 * rad_to_degree * (controller.mouse_pos.y - y) / (float)d.height;
	controller.mouse_pos.y = y;
}

mat4s orthographic_projection_compute() {
	const float SIZE = 18.0;
	const float PLANE_NEAR = 2.0;
	const float PLANE_FAR = 35.5;
	return glms_ortho(-SIZE, SIZE, -SIZE, SIZE, PLANE_NEAR, PLANE_FAR);
}
