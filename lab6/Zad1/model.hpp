#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "pipeline.hpp"
#include "camera.hpp"

class Model {
public:
    Model() = default;
    ~Model();

    void load(std::string path);
    void addMesh(Material material, float x, float y, float z);
    void draw();

//private:

    GLuint mIdVAO;
	GLuint mIdVBO_coord;
    GLuint mIdVBO_color;
    GLuint mIdVBO_uv;

	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec2> mUvs;
	std::vector<glm::vec3> mNormals;

    std::vector<Mesh> mMeshes;

    void createVAO();
    void createVBOCoord();
    void createVBOUV();
    void createVBOColor();
};


class SolidModel : public Model {
public:
	SolidModel() = default;

	void loadTexture(int id, unsigned char *texture, int w, int h, int n);
};


class LightModel : public Model {
public:
    std::vector<LightMesh> mMeshes;

	void addMesh( glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,	glm::vec3 attenuation, glm::vec3 position, bool isPoint = true );

    void draw(Pipeline pipe, glm::mat4 matProj, Camera camera);
};

#endif
