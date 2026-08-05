#include "terrain.h"

void terrain_init(Terrain* terrain) {
	terrain->position = glms_vec2_fill(0.0);
	mesh_terrain_init(&terrain->mesh);
	Material material = {.ambient = {{0.1, 0.1, 0.1}},
		.diffuse = {{0.7, 0.3, 0.1}}, .specular = {{0.3, 0.1, 0.1}},
		.shininess = 1.0};
	terrain->material = material;
	terrain->texture_id = texture_2d_init("grass.png");
}

int fbo_terrain_init(Frame* frame) {
	frame->width = 2048;
	frame->height = 1024;

	glGenFramebuffers(1, &frame->fbo_id);
	glBindFramebuffer(GL_FRAMEBUFFER, frame->fbo_id);

	// Create texture
	glGenTextures(1, &frame->texture_id);
	glBindTexture(GL_TEXTURE_2D, frame->texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame->width, frame->height, 0,
				 GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
						   GL_TEXTURE_2D, frame->texture_id, 0);

	// Create RBO
	glGenRenderbuffers(1, &frame->depth_id);
	glBindRenderbuffer(GL_RENDERBUFFER, frame->depth_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
						  frame->width, frame->height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
							  GL_RENDERBUFFER, frame->depth_id);

	// Create summary
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Error: Framebuffer is not complete!\n");
		exit(1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return 1;
}

void shader_terrain_init(Shader* shader) {
	const int COUNT = 7;
	const int UNIFORM_IDS[7] = {
		UNIFORM_PROJECTION,
		UNIFORM_VIEW,
		UNIFORM_MODEL,
		UNIFORM_LIGHT_PROJECTION,
		UNIFORM_LIGHT_VIEW,
		UNIFORM_SHADOW_TEXTURE,
		UNIFORM_CAMERA_COORDS,
	};
	char UNIFORM_NAMES[7][32] = {
		"matProj",
		"matView",
		"matModel",
		"lightProj",
		"lightView",
		"tex_shadowMap",
		"cameraPos",
	};

	shader_init(shader, "terrain", COUNT, UNIFORM_IDS, UNIFORM_NAMES);
}

void renderer_terrain_init(RendererTerrain* renderer, Shader* shader) {
	renderer->shader = shader;
	terrain_init(&renderer->terrain);
	fbo_terrain_init(&renderer->frame);

	vec3s pos = {{renderer->terrain.position.x, 0, renderer->terrain.position.y}};
	mat4s model = glms_translate(glms_mat4_identity(), pos);

	glUseProgram(renderer->shader->id);
	uniform_material_send(renderer->shader->id, "my_material.", &renderer->terrain.material);
	texture_2d_send(renderer->shader->id, renderer->terrain.texture_id);
	glUniformMatrix4fv(renderer->shader->locations[UNIFORM_MODEL], 1, GL_FALSE, model.raw[0]);
	glUseProgram(0);
}

void renderer_terrain_render(RendererTerrain* renderer) {
	glUseProgram(renderer->shader->id);
	// Render world model detection
	glBindFramebuffer(GL_FRAMEBUFFER, renderer->frame.fbo_id);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mesh_raw_draw(&renderer->terrain.mesh);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Render terrain
	mesh_raw_draw(&renderer->terrain.mesh);
	glUseProgram(0);
}
