#ifndef MESH_HPP
#define MESH_HPP

#include <GL/glew.h>
//#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "material.hpp"
#include "texture.hpp"
#include "model.hpp"

#include "obj_loader.hpp"


#define STANDARD 0
#define OUTLINE 1


// Visible object on scene
class Shape {
public:
	Shape(Model *model);
	Shape(Model *model, glm::vec3 position);
	Shape(Model *model, glm::vec3 position, Texture *texture);
	Shape(Model *model, glm::vec3 position, Material &material);

	Material material_;
	Texture *texture_;

	bool HasTexture();

	glm::mat4 CalculateMatModel(int value = 0);
	glm::mat4 ScaledMatModel(float x, float y, float z);

	void Display(GLuint programID, int value = STANDARD);
	void DisplayOutline(GLuint program_id, int);

	void SendMaterial(GLuint programID);

//private:
	glm::vec3 position_;
	glm::vec3 angle_;

	Model *model_ = nullptr;
};


class Piece: public Shape {
public:
	Piece(int field_id, Model *model, Texture* texture);

	std::string get_field();

	void update_field(std::string field);
	void update_world_position();

	//void update_position(float x, float z);
	void update_position();

	bool is_active = true;
	std::string colour;

private:
	std::string field_;
};

#endif
