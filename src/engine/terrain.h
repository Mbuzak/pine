#ifndef PINE_TERRAIN_H
#define PINE_TERRAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "cglm/struct.h"
#include "entity.h"
#include "texture.h"
#include "display.h"
#include "shader.h"

typedef struct {
	GLuint fbo_id;
	GLuint depth_id;
	GLuint texture_id;
	int width;
	int height;
} Frame;

int frame_terrain_init(Frame* frame);

typedef struct {
	vec2s position;
	Mesh mesh;
	Material material;
	GLuint texture_id;
	Shader* shader;

	Frame frame;
} RendererTerrain;

void shader_terrain_init(Shader* shader);

void renderer_terrain_init(RendererTerrain* renderer, Shader* shader);

void renderer_terrain_render(RendererTerrain* renderer);

#endif
