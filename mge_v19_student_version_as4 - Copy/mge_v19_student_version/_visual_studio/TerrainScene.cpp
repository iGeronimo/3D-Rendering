#include <iostream>

#include <string>

#include "glm.hpp"

#include "GL/glew.h"
#include <SFML/Graphics.hpp>

#include "mge/core/Renderer.hpp"
#include "mge/core/ShaderProgram.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Terrain.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/GameObject.hpp"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/materials/TerrainMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/CameraBehaviour.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/MGEDemo.hpp"
#include "TerrainScene.hpp"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
TerrainScene::TerrainScene() :AbstractGame(), _hud(0)
{
}

void TerrainScene::initialize() {
    //setup the core part
    AbstractGame::initialize();

    //setup the custom part so we can display some text
    std::cout << "Initializing HUD" << std::endl;
    _hud = new DebugHud(_window);
    std::cout << "HUD initialized." << std::endl << std::endl;


}

//build the game _world
void TerrainScene::_initializeScene()
{
    //Change Clear color
    glClearColor(0.f, 0.f, 1.f, 0.f);

    //MESHES

    //load a bunch of meshes we will be using throughout this demo
    //each mesh only has to be loaded once, but can be used multiple times:
    //F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
    Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
    Mesh* cubeMeshF = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");
    Mesh* sphereMeshS = Mesh::load(config::MGE_MODEL_PATH + "sphere_flat.obj");
    Mesh* smallTeapotMeshS = Mesh::load(config::MGE_MODEL_PATH + "teapot_smooth.obj");
    Mesh* plane8k = Mesh::load(config::MGE_MODEL_PATH + "plane_8192.obj");

    //MATERIALS

    //create some materials to display the cube, the plane and the light
    AbstractMaterial* lightMaterial = new ColorMaterial(glm::vec3(1, 1, 1));
    AbstractMaterial* blueMaterial = new ColorMaterial(glm::vec3(0, 0, 1));
    AbstractMaterial* yellowMaterial = new ColorMaterial(glm::vec3(0, 1, 1));
    AbstractMaterial* runicStoneMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "runicfloor.png"));
    AbstractMaterial* yepCOCKMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "yepCOCK.jpg"));
    AbstractMaterial* hollowKnightMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "hollowknight.jpg"));
    AbstractMaterial* dreamShieldMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Dreamshield.png"));
    AbstractMaterial* terrainMaterial = new TerrainMaterial(Texture::load(config::MGE_TERRAIN_PATH + "heightmap.png"), 
        Texture::load(config::MGE_TERRAIN_PATH + "splatmap.png"),
        Texture::load(config::MGE_TERRAIN_PATH + "diffuse1.jpg"),
        Texture::load(config::MGE_TERRAIN_PATH + "diffuse2.jpg"),
        Texture::load(config::MGE_TERRAIN_PATH + "diffuse3.jpg"),
        Texture::load(config::MGE_TERRAIN_PATH + "diffuse4.jpg"));


    //SCENE SETUP
    glm::vec3(cameraOffset) = glm::vec3(0, 0, 25);
    Camera* camera = new Camera("camera", glm::vec3(0, 0, 0));
    camera->rotate(glm::radians(-40.0f), glm::vec3(1, 0, 0));
    _world->add(camera);
    _world->setMainCamera(camera);

    //rotation point
    GameObject* rotatePoint = new GameObject("rotatePoint", glm::vec3(0, 0, 0));
    //_world->add(rotatePoint);

    //add the floor
    GameObject* plane = new GameObject("plane", glm::vec3(0, 0, 0));
    plane->scale(glm::vec3(15, 15, 15));
    plane->rotate(0.f, glm::vec3(1, 0, 0));
    plane->setMesh(plane8k);
    plane->setMaterial(terrainMaterial);
    plane->setBehaviour(new RotatingBehaviour());
    _world->add(plane);

    //add rotating 3d object with texture for showcase
    GameObject* ourHero = new GameObject("ourHero", glm::vec3(0, 1, 0));
    ourHero->setMesh(smallTeapotMeshS);
    ourHero->setMaterial(hollowKnightMaterial);
    ourHero->setBehaviour(new RotatingBehaviour());
    //_world->add(ourHero);

    //add a light. Note that the light does ABSOLUTELY ZIP! NADA ! NOTHING !
    //It's here as a place holder to get you started.
    //Note how the texture material is able to detect the number of lights in the scene
    //even though it doesn't implement any lighting yet!

   
    Light* light = new Light("light", glm::vec3(0, 0, 0));
    light->scale(glm::vec3(1, 1, 1));
    light->setMesh(cubeMeshF);
    light->setMaterial(lightMaterial);
    light->setBehaviour(new KeysBehaviour(25));
    light->setParent(_world);
    _world->add(light);
   

    glm::vec3 diffuseColor = glm::vec3(1, 1, 0);

    glm::vec3 ambientLightColor = 0.5f * glm::vec3(0, 1, 1);
    glm::vec3 directionalLightColor = 1.0f * glm::vec3(1, 1, 0);
    glm::vec3 directionalLightVector = glm::vec3(1, -1, -1);


    camera->setBehaviour(new CameraBehaviour(10, 10,  rotatePoint, cameraOffset));
}



void TerrainScene::_render() {
    AbstractGame::_render();
    _updateHud();
}

void TerrainScene::_updateHud() {
    std::string debugInfo = "";
    debugInfo += std::string("FPS:") + std::to_string((int)_fps) + "\n";

    _hud->setDebugInfo(debugInfo);
    _hud->draw();
}

TerrainScene::~TerrainScene()
{
    //dtor
}