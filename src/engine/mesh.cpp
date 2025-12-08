#include "mesh.hpp"
#include "stb_image.h"

int mesh_raw_init(Mesh* mesh) {
	mesh->type = MESH_RAW;
	mesh->size = 36;

	GLfloat positions[8*3] = {
		1.0f, 1.0f, 1.0f,   // 0
		-1.0f, 1.0f, 1.0f,  // 1
		-1.0f, -1.0f, 1.0f, // 2
		1.0f, -1.0f, 1.0f,  // 3
		1.0f, 1.0f, -1.0f,  // 4
		-1.0f, 1.0f, -1.0f, // 5
		-1.0f, -1.0f, -1.0f,// 6
		1.0f, -1.0f, -1.0f  // 7
	};

	GLuint indices[12*3] = {
		5, 0, 1, 5, 4, 0,
		2, 0, 3, 2, 1, 0,
		7, 0, 4, 7, 3, 0,
		3, 6, 2, 3, 7, 6,
		1, 2, 6, 1, 6, 5,
		4, 5, 6, 4, 6, 7
	};

	glGenVertexArrays(1, &mesh->vao);
	glBindVertexArray(mesh->vao);

	mesh->vbos = new GLuint[MESH_RAW_COUNT];
	glGenBuffers(MESH_RAW_COUNT, mesh->vbos);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbos[MESH_RAW_POSITIONS]);
	glBufferData(GL_ARRAY_BUFFER, 8*3*sizeof(GLfloat), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vbos[MESH_RAW_INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->size * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	
	return 0;
}

int mesh_texture_init(Mesh* mesh, std::string filename) {
	mesh->type = MESH_TEXTURE;

	std::string path = "models/" + filename + ".obj";
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	if (!load_obj(path.c_str(), vertices, uvs, normals)) {
		std::cout << "Error: Mesh path: " << path << " not exists!\n";
		exit(0);
	}
	mesh->size = vertices.size();

	// Create VAO
	glGenVertexArrays(1, &mesh->vao);
	glBindVertexArray(mesh->vao);

	mesh->vbos = new GLuint[MESH_TEXTURE_COUNT];
	glGenBuffers(MESH_TEXTURE_COUNT, mesh->vbos);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbos[MESH_TEXTURE_POSITIONS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbos[MESH_TEXTURE_UV_COORDS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs.size(), &uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbos[MESH_TEXTURE_NORMALS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
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

bool load_obj(const char* path, std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals) {
	printf("Loading OBJ file %s ... ", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE* file = fopen(path, "r");
	if(file == NULL){
		printf("can't open the file.\n");
		return false;
	}

	while (1) {
		char lineHeader[128];

		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);

		if (res == EOF)
			break;

		// read vertex coordinates
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		// read texture coordinates
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			// Invert V coordinate since we will only use DDS texture,
			// which are inverted. Remove if you want to use TGA or BMP loaders.
			// uv.y = -uv.y;
			temp_uvs.push_back(uv);
		}
		// read normal vectors
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		// read faces (triangles)
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
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
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);
	}
	fclose(file);
	printf(" done.\n");
	return true;
}
