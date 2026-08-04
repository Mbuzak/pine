#include "mesh.hpp"
#include "stb_image.h"

void attribute_vec2_enable(Mesh* mesh, GLuint index, GLint size, vec2s* data) {
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbos[index]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2s) * size, data, GL_STATIC_DRAW);
	glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(index);
}

void attribute_vec3_enable(Mesh* mesh, GLuint index, GLint size, vec3s* data) {
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbos[index]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3s) * size, data, GL_STATIC_DRAW);
	glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(index);
}

void ibo_enable(Mesh* mesh, GLint ibo_size, GLuint* indices) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_size * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

void mesh_cube_init(Mesh* mesh) {
	const int vertex_count = 24;
	const int triangle_count = 12;
	vec3s coords[vertex_count] = {
		{{-1.0, -1.0, 1.0}},
		{{-1.0, 1.0, 1.0}},
		{{1.0, -1.0, 1.0}},
		{{1.0, 1.0, 1.0}},
		{{-1.0, -1.0, -1.0}},
		{{-1.0, 1.0, -1.0}},
		{{1.0, -1.0, -1.0}},
		{{1.0, 1.0, -1.0}},
		{{-1.0, -1.0, -1.0}},
		{{-1.0, -1.0, 1.0}},
		{{1.0, -1.0, -1.0}},
		{{1.0, -1.0, 1.0}},
		{{-1.0, 1.0, -1.0}},
		{{-1.0, 1.0, 1.0}},
		{{1.0, 1.0, -1.0}},
		{{1.0, 1.0, 1.0}},
		{{-1.0, -1.0, -1.0}},
		{{-1.0, -1.0, 1.0}},
		{{-1.0, 1.0, -1.0}},
		{{-1.0, 1.0, 1.0}},
		{{1.0, -1.0, -1.0}},
		{{1.0, -1.0, 1.0}},
		{{1.0, 1.0, -1.0}},
		{{1.0, 1.0, 1.0}},
	};
	vec2s uv_coords[vertex_count] = {
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
		{{0.0, 0.0}},
	};
	vec3s normals[vertex_count] = {
		{{0.0, 0.0, -1.0}},
		{{0.0, 0.0, -1.0}},
		{{0.0, 0.0, -1.0}},
		{{0.0, 0.0, -1.0}},
		{{0.0, 0.0, 1.0}},
		{{0.0, 0.0, 1.0}},
		{{0.0, 0.0, 1.0}},
		{{0.0, 0.0, 1.0}},
		{{0.0, -1.0, 0.0}},
		{{0.0, -1.0, 0.0}},
		{{0.0, -1.0, 0.0}},
		{{0.0, -1.0, 0.0}},
		{{0.0, 1.0, 0.0}},
		{{0.0, 1.0, 0.0}},
		{{0.0, 1.0, 0.0}},
		{{0.0, 1.0, 0.0}},
		{{-1.0, 0.0, 0.0}},
		{{-1.0, 0.0, 0.0}},
		{{-1.0, 0.0, 0.0}},
		{{-1.0, 0.0, 0.0}},
		{{1.0, 0.0, 0.0}},
		{{1.0, 0.0, 0.0}},
		{{1.0, 0.0, 0.0}},
		{{1.0, 0.0, 0.0}},
	};
	GLuint indices[triangle_count * 3] = {
		0, 1, 2, 1, 2, 3,
		4, 5, 7, 4, 6, 7,
		8, 9, 11, 8, 10, 11,
		12, 13, 14, 13, 14, 15,
		16, 17, 18, 17, 18, 19,
		20, 21, 23, 20, 22, 23,
	};
	mesh->size = triangle_count * 3;

	glGenVertexArrays(1, &mesh->vao);
	mesh->vbos = new GLuint[3];
	glGenBuffers(3, mesh->vbos);
	glGenBuffers(1, &mesh->ibo);

	glBindVertexArray(mesh->vao);
	attribute_vec3_enable(mesh, 0, vertex_count, coords);
	attribute_vec2_enable(mesh, 1, vertex_count, uv_coords);
	attribute_vec3_enable(mesh, 2, vertex_count, normals);
	ibo_enable(mesh, mesh->size, indices);
	glBindVertexArray(0);
}

void mesh_terrain_init(Mesh* mesh) {
	const int vertex_count = 4;
	const int triangle_count = 2;
	vec3s coords[vertex_count] = {
		{{-80.0,  0.0, -80.0}},
		{{-80.0,  0.0, 80.0}},
		{{80.0, 0.0, -80.0}},
		{{80.0, 0.0, 80.0}},
	};
	vec2s uv_coords[vertex_count] = {
		{{0.0, 0.0}},
		{{0.0, 1.0}},
		{{1.0, 0.0}},
		{{1.0, 1.0}},
	};
	vec3s normals[vertex_count] = {
		{{0.0, 1.0, 0.0}},
		{{0.0, 1.0, 0.0}},
		{{0.0, 1.0, 0.0}},
		{{0.0, 1.0, 0.0}},
	};
	GLuint indices[triangle_count * 3] = {
		0, 1, 2,
		1, 2, 3
	};
	mesh->size = triangle_count * 3;

	glGenVertexArrays(1, &mesh->vao);
	mesh->vbos = new GLuint[3];
	glGenBuffers(3, mesh->vbos);
	glGenBuffers(1, &mesh->ibo);

	glBindVertexArray(mesh->vao);
	attribute_vec3_enable(mesh, 0, vertex_count, coords);
	attribute_vec2_enable(mesh, 1, vertex_count, uv_coords);
	attribute_vec3_enable(mesh, 2, vertex_count, normals);
	ibo_enable(mesh, mesh->size, indices);
	glBindVertexArray(0);
}

void mesh_raw_draw(Mesh* mesh) {
	glBindVertexArray(mesh->vao);
	glDrawElements(GL_TRIANGLES, mesh->size, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void mesh_texture_draw(Mesh* mesh) {
	glBindVertexArray(mesh->vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh->size);
	glBindVertexArray(0);
}
