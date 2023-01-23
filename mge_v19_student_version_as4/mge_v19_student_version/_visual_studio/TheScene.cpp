#include <iostream>

#include <string>

#include "glm.hpp"

#include "GL/glew.h"

#include "mge/core/Renderer.hpp"
#include "mge/core/ShaderProgram.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/GameObject.hpp"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/CameraBehaviour.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/MGEDemo.hpp"
#include "TheScene.hpp"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
TheScene::TheScene() :AbstractGame(), _hud(0)
{
}

void TheScene::initialize() {
    //setup the core part
    AbstractGame::initialize();

    //setup the custom part so we can display some text
    std::cout << "Initializing HUD" << std::endl;
    _hud = new DebugHud(_window);
    std::cout << "HUD initialized." << std::endl << std::endl;

    
}

//build the game _world
void TheScene::_initializeScene()
{
    //MESHES

    //load a bunch of meshes we will be using throughout this demo
    //each mesh only has to be loaded once, but can be used multiple times:
    //F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
    Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
    //Mesh* cubeMeshF = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");
    Mesh* sphereMeshF = Mesh::load(config::MGE_MODEL_PATH + "sphere_flat.obj");
    Mesh* smallTeapotMeshS = Mesh::load(config::MGE_MODEL_PATH + "teapot_smooth.obj");

    //MATERIALS

    //create some materials to display the cube, the plane and the light
    AbstractMaterial* lightMaterial = new ColorMaterial(glm::vec3(1, 1, 0));
    AbstractMaterial* blueMaterial = new ColorMaterial(glm::vec3(0, 0, 1));
    AbstractMaterial* yellowMaterial = new ColorMaterial(glm::vec3(0, 1, 1));
    AbstractMaterial* runicStoneMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "runicfloor.png"));
    AbstractMaterial* yepCOCKMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "yepCOCK.jpg"));
    AbstractMaterial* hollowKnightMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "hollowknight.jpg"));
    AbstractMaterial* dreamShieldMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Dreamshield.png"));

    

    //SCENE SETUP

   //add camera first (it will be updated last)
    glm::vec3(cameraOffset) = glm::vec3(0, 0, 10);
    Camera* camera = new Camera("camera", cameraOffset);
    _world->setMainCamera(camera);
    _world->add(camera);
   
    //add the floor
    
    GameObject* plane = new GameObject("plane", glm::vec3(0, 0, 0));
    plane->scale(glm::vec3(5, 5, 5));
    plane->setMesh(planeMeshDefault);
    plane->setMaterial(runicStoneMaterial);
    _world->add(plane);
    
    

    //add a spinning sphere
    GameObject* sphere = new GameObject("sphere", glm::vec3(0, 0, 0));
    sphere->scale(glm::vec3(2.5, 2.5, 2.5));
    sphere->setMesh(sphereMeshF);
    sphere->setMaterial(hollowKnightMaterial);
    sphere->setBehaviour(new KeysBehaviour());
    _world->add(sphere);


    GameObject* smallTeapotYep = new GameObject("smallTeapotYep", glm::vec3(2, 1, 0));
    smallTeapotYep->scale(glm::vec3(0.2, 0.2, 0.2));
    smallTeapotYep->setMesh(smallTeapotMeshS);
    smallTeapotYep->setMaterial(yepCOCKMaterial);
    smallTeapotYep->setBehaviour(new RotatingBehaviour());
    _world->add(smallTeapotYep);

    GameObject* smallTeapotYellow = new GameObject("smallTeapotYellow", glm::vec3(0, 1.5, 0));
    smallTeapotYellow->scale(glm::vec3(0.2, 0.2, 0.2));
    smallTeapotYellow->setMesh(smallTeapotMeshS);
    smallTeapotYellow->setMaterial(yellowMaterial);
    smallTeapotYellow->setBehaviour(new RotatingBehaviour());
    _world->add(smallTeapotYellow);

    GameObject* smallTeapotBlu = new GameObject("smallTeapotBlue", glm::vec3(-2, 1, 0));
    smallTeapotBlu->scale(glm::vec3(0.2, 0.2, 0.2));
    smallTeapotBlu->setMesh(smallTeapotMeshS);
    smallTeapotBlu->setMaterial(blueMaterial);
    smallTeapotBlu->setBehaviour(new RotatingBehaviour());
    smallTeapotBlu->setParent(sphere);


    //camera->rotate(glm::radians(-40.0f), glm::vec3(1, 0, 0));
    camera->setBehaviour(new CameraBehaviour(10, 10, sphere, cameraOffset));



    //add a light. Note that the light does ABSOLUTELY ZIP! NADA ! NOTHING !
    //It's here as a place holder to get you started.
    //Note how the texture material is able to detect the number of lights in the scene
    //even though it doesn't implement any lighting yet!

    /*
    Light* light = new Light("light", glm::vec3(0, 4, 0));
    light->scale(glm::vec3(0.1f, 0.1f, 0.1f));
    light->setMesh(cubeMeshF);
    light->setMaterial(lightMaterial);
    light->setBehaviour(new KeysBehaviour(25));
    _world->add(light);
    */

}



void TheScene::_render() {
    AbstractGame::_render();
    _updateHud();
}

void TheScene::_updateHud() {
    std::string debugInfo = "";
    debugInfo += std::string("FPS:") + std::to_string((int)_fps) + "\n";

    _hud->setDebugInfo(debugInfo);
    _hud->draw();
}

TheScene::~TheScene()
{
    //dtor
}


