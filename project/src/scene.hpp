#ifndef SCENE_HPP
#define SCENE_HPP

#include <time.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <filesystem>

#include "engine/model.hpp"
#include "engine/mesh.hpp"
#include "engine/light.hpp"
#include "engine/pipeline.hpp"
#include "engine/texture.hpp"
#include "engine/shadow.hpp"
#include "engine/camera.hpp"
#include "engine/framebuffer.hpp"
#include "game.hpp"

#include "utilities.hpp"


class Scene {
public:
	Scene();

	// Loads all files from folder
	void Load(const char *folder);

	void Setup();
	void AddModel(const char *name);
	void AddTexture(const char *filename, const char *extension);
	void Display();

	void SendLight();

//private:
	//glm::mat4 matrix_projection_;

	GLuint sbp;
	GLuint program_default;

	Skybox *skybox_ = nullptr;
	Camera camera_;

	std::map<std::string, Model*> models_;
	std::map<std::string, Texture*> textures_;

	// --- meshes ---
	Sun *sun_ = nullptr;
	std::array<Lamp*, 4> lamps_;

	std::vector<Shape*> background_;

	Game* game_;

	ShadowMap *dir_shadow_map_;

	Framebuffer *fbo = nullptr;

private:
	void RenderShadowMapOfDirectionalLight();

	void RenderToTexture();

	void RenderSkybox();
	void RenderShapes();
	void RenderLights();
};

#endif
