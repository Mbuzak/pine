#ifndef SCENE_HPP
#define SCENE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <time.h>
#include <stdlib.h>
#include "mesh.hpp"
#include "model.hpp"
#include "pipeline.hpp"
#include "camera.hpp"
#include "material.hpp"

#define PHONG_LIGHT 0
#define BLINN_PHONG 1

#define DIRECTION_OFF 0
#define DIRECTION_ON 1

#define LIGHT_OFF 0
#define LIGHT_ON 1

class Scene {
public:
    bool reflectionLight = PHONG_LIGHT;
    bool light = LIGHT_ON;
    bool direction = DIRECTION_OFF;

    Scene();

    void setup();
    void addModel(const char *path, const char *name);
    void addLightModel(const char *path, const char *name);
    void display(glm::mat4 matProj);

    void sendFlags();
    void sendLight();
    void sendMaterial(Mesh &mesh);

//private:
    Pipeline mPipe;
    std::map<std::string, SolidModel> mModels;
    LightModel mLightModel;
    Camera mCamera;
    Material mMaterial;
};

#endif
