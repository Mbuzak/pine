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

int mesh_raw_init(Mesh* mesh) {
	mesh->size = 36;

	vec3s positions[8] = {
		{{1.0, 1.0, 1.0}},
		{{-1.0, 1.0, 1.0}},
		{{-1.0, -1.0, 1.0}},
		{{1.0, -1.0, 1.0}},
		{{1.0, 1.0, -1.0}},
		{{-1.0, 1.0, -1.0}},
		{{-1.0, -1.0, -1.0}},
		{{1.0, -1.0, -1.0}}
	};

	GLuint indices[12*3] = {
		5, 0, 1,
		5, 4, 0,
		2, 0, 3,
		2, 1, 0,
		7, 0, 4,
		7, 3, 0,
		3, 6, 2,
		3, 7, 6,
		1, 2, 6,
		1, 6, 5,
		4, 5, 6,
		4, 6, 7
	};

	glGenVertexArrays(1, &mesh->vao);

	mesh->vbos = new GLuint[1];
	glGenBuffers(1, mesh->vbos);
	glGenBuffers(1, &mesh->ibo);

	glBindVertexArray(mesh->vao);
	attribute_vec3_enable(mesh, 0, 8, positions);
	ibo_enable(mesh, mesh->size, indices);
	glBindVertexArray(0);

	return 0;
}

int mesh_texture_init(Mesh* mesh, const char* filename) {
	char path[32];
	strcpy(path, "res/models/");
	strcat(path, filename);
	strcat(path, ".obj");
	std::vector<vec3s> vertices;
	std::vector<vec2s> uvs;
	std::vector<vec3s> normals;
	if (!load_obj(path, vertices, uvs, normals)) {
		printf("Error: Mesh path: %s not exists!\n", path);
		exit(0);
	}
	mesh->size = vertices.size();

	// Create VAO
	glGenVertexArrays(1, &mesh->vao);
	glBindVertexArray(mesh->vao);

	mesh->vbos = new GLuint[MESH_TEXTURE_COUNT];
	glGenBuffers(MESH_TEXTURE_COUNT, mesh->vbos);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbos[MESH_TEXTURE_POSITIONS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3s) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbos[MESH_TEXTURE_UV_COORDS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2s) * uvs.size(), &uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbos[MESH_TEXTURE_NORMALS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3s) * normals.size(), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	return 0;
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

bool load_obj(const char* path, std::vector<vec3s>& out_vertices,
	std::vector<vec2s>& out_uvs, std::vector<vec3s>& out_normals) {
	printf("Loading OBJ file %s ... ", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<vec3s> temp_vertices;
	std::vector<vec2s> temp_uvs;
	std::vector<vec3s> temp_normals;

	FILE* file = fopen(path, "r");
	if(file == NULL){
		printf("can't open the file.\n");
		return false;
	}

	while (1) {
		char lineHeader[128];

		// read the first word of the line
		if (fscanf(file, "%s", lineHeader) == EOF) {
			break;
		}

		// read vertex coordinates
		if (strcmp(lineHeader, "v") == 0) {
			vec3s vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		// read texture coordinates
		else if (strcmp(lineHeader, "vt") == 0) {
			vec2s uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		// read normal vectors
		else if (strcmp(lineHeader, "vn") == 0) {
			vec3s normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		// read faces (triangles)
		else if (strcmp(lineHeader, "f") == 0) {
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
									&vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			if (matches != 9) {
				printf("\nFile can't be read by our simple parser. ");
				printf("Try exporting with other options and make sure you export normals and uvs.\n");
				fclose(file);
				return false;
			}

			// Create new triangle
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	// For each vertex of each triangle
	for(unsigned int i=0; i<vertexIndices.size(); i++){
		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		vec3s vertex = temp_vertices[ vertexIndex-1 ];
		vec2s uv = temp_uvs[ uvIndex-1 ];
		vec3s normal = temp_normals[ normalIndex-1 ];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);
	}
	fclose(file);
	printf(" done.\n");
	return true;
}
