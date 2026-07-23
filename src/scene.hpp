#ifndef PINE_SCENE
#define PINE_SCENE

#define TEX_WHT 0
#define TEX_BLC 1

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include "engine/mesh.hpp"
#include "engine/entity.hpp"
#include "engine/light.hpp"
#include "engine/shadow.hpp"
#include "engine/camera.hpp"
#include "engine/renderer.hpp"
#include "engine/skybox.hpp"
#include "engine/rendered.hpp"
#include "engine/uniform.hpp"
#include "SDL2/SDL.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "engine/shader.h"
#include "engine/display.h"
#include "engine/frame.h"
#include "engine/controller.h"
#include "engine/texture.h"
#ifdef __cplusplus
}
#endif

#define __CHECK_FOR_ERRORS 	{GLenum errCode; if ((errCode = glGetError()) != GL_NO_ERROR) printf("Error (%d): %s in file %s at line %d !\n", errCode, gluErrorString(errCode), __FILE__,  __LINE__);}

class Scene {
public:
	Scene() = default;

	Display d;
	RendererSkybox renderer_skybox;

	GLuint program_color;
	ShaderRendered shader_rendered;

	Camera camera;
	CameraOrthographic camera_light;
	Controller controller;

	std::map<std::string, Mesh> meshes;
	GLuint* textures;

	Sun sun;
	std::array<Lamp, 4> lamps;
	Shape terrain;
	std::vector<Shape*> pieces_;

	ShadowMap dir_shadow_map;
	Frame frame;

	int selected_id = -1;

private:
	void RenderToTexture(GLuint);
	void RenderShapes(GLuint);

	int events_handle();
	void select_piece(int wx, int wy, int x, int y);
	void motion(int x, int y);
	void reshape(int, int);
	void rotate(int, int);

public:
	void Setup();
	void display();
};

void lamps_render(std::array<Lamp, 4>, GLuint);

glm::mat4 perspective_projection_compute(float, float);

#endif
