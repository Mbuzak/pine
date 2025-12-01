#include "scene.hpp"

Scene::Scene() {
    mMaterial = Material{
        glm::vec3 (0.1 , 0.1 , 0.1), // ambient
        glm::vec3 (1.0 , 1.0 , 1.0), // diffuse
        glm::vec3 (1.5 , 1.5 , 1.5), // specular
        1.0 // shininess
    };
}

void Scene::setup() {
    mPipe.createProgram();
	mPipe.loadShaders("vertex.glsl", "fragment.glsl");

    // stbi here( wczytywanie tekstur )

    addModel("ground-large.obj", "Ground");
    addModel("christmas-tree.obj", "Trees");
    addModel("cube.obj", "Cubes");

    mLightModel.load("sphere.obj");

    int number = 20;
    int density = 8;
    for( int i = 0; i < number; i++ ) {
        int x = ( rand() % 2 == 1 )? rand() % density : -rand() % density;
        int z = ( rand() % 2 == 1 )? rand() % density : -rand() % density;

        mModels.at("Trees").addMesh(mMaterial, x * 2, 0.0, z * 2);
    }

    Material bronzeM = Material{glm::vec3 (0.2125f, 0.1275f, 0.054f), glm::vec3 (0.714f, 0.4284f, 0.18144f), glm::vec3 (0.393548f, 0.271906f, 0.166721f), 10.6};
    Material silverM = Material{glm::vec3 (0.19225f, 0.19225f, 0.19225f), glm::vec3 (0.50754f, 0.50754f, 0.50754f), glm::vec3 (0.508273f, 0.508273f, 0.508273f), 1.2f};
    Material rubyM = Material{glm::vec3 (0.9745f, 0.01175f, 0.01175f), glm::vec3 (0.61424f, 0.04136f, 0.04136f), glm::vec3 (0.727811f, 0.626959f, 0.626959f), 1.8f};
    Material copperM = Material{glm::vec3 (0.19125f, 0.0735f, 0.9225f), glm::vec3 (0.7038f, 0.27048f, 0.0828f), glm::vec3 (0.256777f, 0.137622f, 0.086014f), 1.8f};

    mModels.at("Cubes").addMesh(bronzeM, 3.0, 1.2, 0.0);
    mModels.at("Cubes").addMesh(silverM, -3.0, 1.2, 0.0);
    mModels.at("Cubes").addMesh(rubyM, 0.0, 1.2, 3.0);
    mModels.at("Cubes").addMesh(copperM, 0.0, 1.2, -3.0);

    mModels.at("Ground").addMesh(mMaterial, 0.0, 0.75, 0.0);

    mLightModel.addMesh( glm::vec3(0.9 , 0.9 , 0.9), glm::vec3 (0.2 , 0.2 , 0.9), glm::vec3 (0.1 , 0.5 , 0.9), glm::vec3 (0.1 , 0.0 , 0.05), glm::vec3 (0.0 , -1.0 , 0.0), false );

    mLightModel.addMesh( glm::vec3(0.5 , 0.5 , 0.5), glm::vec3 (0.2 , 0.9 , 0.2), glm::vec3 (0.1 , 0.2 , 0.2), glm::vec3 (1.0 , 0.0 , 0.1), glm::vec3 (0.0 , 3.0 , 15.0) );
    mLightModel.addMesh( glm::vec3(0.1 , 0.1 , 0.1), glm::vec3 (0.5 , 0.7 , 0.2), glm::vec3 (0.1 , 0.2 , 0.3), glm::vec3 (1.0 , 0.0 , 0.1), glm::vec3 (8.0 , 2.0 , 4.0) );
    mLightModel.addMesh( glm::vec3(0.1 , 0.1 , 0.1), glm::vec3 (0.7 , 0.5 , 0.3), glm::vec3 (0.1 , 0.2 , 0.2), glm::vec3 (1.0 , 0.0 , 0.1), glm::vec3 (-8.0 , 2.0 , -8.0) );
    mLightModel.addMesh( glm::vec3(0.1 , 0.1 , 0.1), glm::vec3 (0.5 , 0.2 , 0.0), glm::vec3 (0.1 , 0.1 , 0.2), glm::vec3 (1.0 , 0.0 , 0.1), glm::vec3 (2.0 , 2.0 , -8.0) );
    mLightModel.addMesh( glm::vec3(0.1 , 0.1 , 0.1), glm::vec3 (0.9 , 0.1 , 0.3), glm::vec3 (0.1 , 0.2 , 0.2), glm::vec3 (1.0 , 0.0 , 0.1), glm::vec3 (8.0 , 2.0 , 8.0) );
    mLightModel.addMesh( glm::vec3(0.1 , 0.1 , 0.1), glm::vec3 (0.9 , 0.2 , 0.5), glm::vec3 (0.1 , 0.3 , 0.2), glm::vec3 (1.0 , 0.0 , 0.1), glm::vec3 (8.0 , 2.0 , 0.0) );
    mLightModel.addMesh( glm::vec3(0.1 , 0.1 , 0.1), glm::vec3 (0.7 , 0.1 , 0.1), glm::vec3 (0.1 , 0.2 , 0.3), glm::vec3 (1.0 , 0.0 , 0.1), glm::vec3 (0.0 , 2.0 , 8.0) );
    

    std::cout << std::endl;
    std::cout << "GLUT menu nie działa\n";
    std::cout << "Link z podobnym problemem: https://www.reddit.com/r/opengl/comments/d7cms3/glut_menus_not_showing_up_details_in_comment/" << "\n\n";

    std::cout << "--- Skroty klawiszowe ---\n\n";
    std::cout << "(1) Swiatlo odbiciowe w modelu Phonga\n";
    std::cout << "(2) Swiatlo odbiciowe w modelu Blinna-Phonga\n";
    std::cout << std::endl;

    std::cout << "(3) Wyłaczenie swiatla\n";
    std::cout << "(4) Wlaczenie swiatla\n";
    std::cout << std::endl;

    std::cout << "(5) Wyłaczenie animacji swiatla\n";
    std::cout << "(6) Wlaczenie animacji swiatla\n";
    std::cout << std::endl;

    std::cout << "(7) Wyłaczenie swiatla kierunkowego\n";
    std::cout << "(8) Wlaczenie swiatla kierunkowego\n";
    std::cout << std::endl;
}

void Scene::addModel(const char *path, const char *name) {
    mModels.insert(std::pair<std::string, SolidModel>(name, SolidModel()));
    mModels.at(name).load(path);
}

void Scene::display(glm::mat4 matProj) {
    mCamera.update();

    mPipe.activate();

    sendFlags();
	sendLight();

    // Rendering
    glUniform1i(glGetUniformLocation(mPipe.mId, "isLight"), false);
    for( auto &[name, model] : mModels ) {
        for( auto &mesh : model.mMeshes ) {
            sendMaterial(mesh);

            mPipe.sendVertices(mesh.calculateMatModel(), mCamera.getMatView(), matProj, mCamera.getPosition());

            glUniform1i(glGetUniformLocation(mPipe.mId, "hasTex"), mesh.hasTex());

            if( mesh.hasTex() ) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, model.mIdVBO_uv);
                glUniform1i(glGetUniformLocation(mPipe.mId, "tex"), 0);
            }

	        model.draw();
        }
    }

    mLightModel.draw(mPipe, matProj, mCamera);

	mPipe.disactivate();
}

void Scene::sendFlags() {
    GLint idProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &idProgram);

    glUniform1i(glGetUniformLocation(idProgram, "reflectionLight"), reflectionLight);
    glUniform1i(glGetUniformLocation(idProgram, "Light"), light);
    glUniform1i(glGetUniformLocation(idProgram, "Direction"), direction);
}

void Scene::sendLight() {
	GLint idProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &idProgram);

    // directional light
    glUniform3fv(glGetUniformLocation(idProgram, "dirLight.Ambient"), 1, glm::value_ptr(mLightModel.mMeshes[0].Ambient));
    glUniform3fv(glGetUniformLocation(idProgram, "dirLight.Diffuse"), 1, glm::value_ptr(mLightModel.mMeshes[0].Diffuse));
    glUniform3fv(glGetUniformLocation(idProgram, "dirLight.Specular"), 1, glm::value_ptr(mLightModel.mMeshes[0].Specular));
    glUniform3fv(glGetUniformLocation(idProgram, "dirLight.Attenuation"), 1, glm::value_ptr(mLightModel.mMeshes[0].Attenuation));
    glUniform3fv(glGetUniformLocation(idProgram, "dirLight.Position"), 1, glm::value_ptr(mLightModel.mMeshes[0].Position));

    // point lights
    for ( int i = 1 ; i < mLightModel.mMeshes.size() ; i++ ) {
        std::string ambient = "lights[" + std::to_string(i - 1) + "].Ambient";
        std::string diffuse = "lights[" + std::to_string(i - 1) + "].Diffuse";
        std::string specular = "lights[" + std::to_string(i - 1) + "].Specular";
        std::string attenuation = "lights[" + std::to_string(i - 1) + "].Attenuation";
        std::string position = "lights[" + std::to_string(i - 1) + "].Position";

        glUniform3fv(glGetUniformLocation(idProgram, ambient.c_str()), 1, glm::value_ptr(mLightModel.mMeshes[i].Ambient));
        glUniform3fv(glGetUniformLocation(idProgram, diffuse.c_str()), 1, glm::value_ptr(mLightModel.mMeshes[i].Diffuse));
        glUniform3fv(glGetUniformLocation(idProgram, specular.c_str()), 1, glm::value_ptr(mLightModel.mMeshes[i].Specular));
        glUniform3fv(glGetUniformLocation(idProgram, attenuation.c_str()), 1, glm::value_ptr(mLightModel.mMeshes[i].Attenuation));
        glUniform3fv(glGetUniformLocation(idProgram, position.c_str()), 1, glm::value_ptr(mLightModel.mMeshes[i].Position));
    }
}

void Scene::sendMaterial(Mesh &mesh) {
	GLint idProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &idProgram);

	glUniform3fv(glGetUniformLocation(idProgram, "myMaterial.Ambient"), 1, glm::value_ptr(mesh.mMaterial.Ambient));
	glUniform3fv(glGetUniformLocation(idProgram, "myMaterial.Diffuse"), 1, glm::value_ptr(mesh.mMaterial.Diffuse));
	glUniform3fv(glGetUniformLocation(idProgram, "myMaterial.Specular"), 1, glm::value_ptr(mesh.mMaterial.Specular));
	glUniform1f(glGetUniformLocation(idProgram, "myMaterial.Shininess"), mesh.mMaterial.Shininess);
}
