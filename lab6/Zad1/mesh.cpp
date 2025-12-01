#include "mesh.hpp"

Mesh::Mesh(Material material, float x, float y, float z):
mPosition(glm::vec3{x, y, z}), mAngle(glm::vec3{0.0, 0.0, 0.0}) {
	mMaterial = material;
}

bool Mesh::hasTex() {
	return tx != NULL;
}

glm::mat4 Mesh::calculateMatModel() {
	glm::mat4 model(1.0);

	model = glm::translate(model, mPosition);
	model = glm::rotate(model, mAngle.x, glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, mAngle.y, glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, mAngle.z, glm::vec3(0.0, 0.0, 1.0));

	return model;
}

glm::mat4 Mesh::scaledMatModel(float x, float y, float z) {
	return glm::scale(calculateMatModel(), glm::vec3(x, y, z));
}

LightMesh::LightMesh( glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,	glm::vec3 attenuation, glm::vec3 position, bool isPoint ) {
	Ambient = ambient;
	Diffuse = diffuse;
	Specular = specular;
	Attenuation = attenuation;
	Position = position;

	mAngle = glm::vec3(0.0, 0.0, 0.0);

	mIsPoint = isPoint;
}

glm::mat4 LightMesh::calculateMatModel() {
	glm::mat4 model(1.0);

	model = glm::translate(model, Position);
	model = glm::rotate(model, mAngle.x, glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, mAngle.y, glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, mAngle.z, glm::vec3(0.0, 0.0, 1.0));

	return model;
}
