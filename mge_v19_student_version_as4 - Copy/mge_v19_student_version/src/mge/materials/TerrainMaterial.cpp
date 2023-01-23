#include "glm.hpp"

#include "mge/core/Camera.hpp"
#include "mge/materials/TerrainMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/config.hpp"

ShaderProgram* TerrainMaterial::_shader = NULL;

GLint TerrainMaterial::_uMVPMatrix = 0;

GLint TerrainMaterial::_uHeightMap = 0;
GLint TerrainMaterial::_uSplatMap = 0;

GLint TerrainMaterial::_uDiffuseTexture1 = 0;
GLint TerrainMaterial::_uDiffuseTexture2 = 0;
GLint TerrainMaterial::_uDiffuseTexture3 = 0;
GLint TerrainMaterial::_uDiffuseTexture4 = 0;

GLint TerrainMaterial::_aVertex = 0;
GLint TerrainMaterial::_aNormal = 0;
GLint TerrainMaterial::_aUV = 0;

TerrainMaterial::TerrainMaterial(Texture* pHeightMap, Texture* pSplatMap, Texture* pDiffuseTexture1, Texture* pDiffuseTexture2, Texture* pDiffuseTexture3, Texture* pDiffuseTexture4) :_heightMap(pHeightMap), _splatMap(pSplatMap),_diffuseTexture1(pDiffuseTexture1), _diffuseTexture2(pDiffuseTexture2), _diffuseTexture3(pDiffuseTexture3), _diffuseTexture4(pDiffuseTexture4) {
    _lazyInitializeShader();
}

TerrainMaterial::~TerrainMaterial() {}

void TerrainMaterial::_lazyInitializeShader() {
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "terrain.vert");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "terrain.frag");
        _shader->finalize();

        //cache all the uniform and attribute indexes
        _uMVPMatrix = _shader->getUniformLocation("mvpMatrix");

        _uHeightMap = _shader->getUniformLocation("heightMap");
        _uSplatMap = _shader->getUniformLocation("splatMap");

        _uDiffuseTexture1 = _shader->getUniformLocation("diffuseTexture1");
        _uDiffuseTexture2 = _shader->getUniformLocation("diffuseTexture2");
        _uDiffuseTexture3 = _shader->getUniformLocation("diffuseTexture3");
        _uDiffuseTexture4 = _shader->getUniformLocation("diffuseTexture4");

        _aVertex = _shader->getAttribLocation("vertex");
        _aNormal = _shader->getAttribLocation("normal");
        _aUV = _shader->getAttribLocation("uv");
    }
}

void TerrainMaterial::setTextureHeightMap(Texture* pHeightMap) {
    _heightMap = pHeightMap;
}

void TerrainMaterial::setTextureSplatMap(Texture* pSplatMap) {
    _splatMap = pSplatMap;
}

void TerrainMaterial::setDiffuseTexture1(Texture* pDiffuseTexture1) {
    _diffuseTexture1 = pDiffuseTexture1;
}

void TerrainMaterial::setDiffuseTexture2(Texture* pDiffuseTexture2) {
    _diffuseTexture2 = pDiffuseTexture2;
}

void TerrainMaterial::setDiffuseTexture3(Texture* pDiffuseTexture3) {
    _diffuseTexture3 = pDiffuseTexture3;
}

void TerrainMaterial::setDiffuseTexture4(Texture* pDiffuseTexture4) {
    _diffuseTexture4 = pDiffuseTexture4;
}

void TerrainMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    if (!_diffuseTexture1 || !_diffuseTexture2 || !_diffuseTexture3 || !_diffuseTexture4 || !_heightMap || !_splatMap) {
        std::cout << "One of Terrains not found" << std::endl;
        return;
    }

    //std::cout << "All Terrains found" << std::endl;
    

    _shader->use();



    //Print the number of lights in the scene and the position of the first light.
    //It is not used, but this demo is just meant to show you THAT materials can access the lights in a world
    if (pWorld->getLightCount() > 0) {
        std::cout << "TextureMaterial has discovered light is at position:" << pWorld->getLightAt(0)->getLocalPosition() << std::endl;

        //send light position to texture shader
        glUniform3fv(_shader->getUniformLocation("lightPos0"), 1, glm::value_ptr(pWorld->getLightAt(0)->getLocalPosition()));

        //currenct active camera
        Camera* camera = pWorld->getMainCamera();

        //send camera position to texture shader
        glUniform3fv(_shader->getUniformLocation("cameraPos"), 1, glm::value_ptr(camera->getLocalPosition()));
    }

    //setup texture slot 0
    glActiveTexture(GL_TEXTURE0);
    //bind the texture to the current active slot
    glBindTexture(GL_TEXTURE_2D, _heightMap->getId());
    //tell the shader the texture slot for the diffuse texture is slot 0
    glUniform1i(_uHeightMap, 0);

    //pass in height constant
    float heightConstant = 1;
    glUniform1f(_shader->getUniformLocation("heightConstant"), heightConstant);


    //pass in a precalculate mvp matrix (see texture material for the opposite)
    glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
    glUniformMatrix4fv(_shader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(pModelMatrix));
    glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

    //now inform mesh of where to stream its data
    pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);

    //setup texture slot 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _splatMap->getId());
    glUniform1i(_uSplatMap, 1);

    //setup texture slot 2
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture1->getId());
    glUniform1i(_uDiffuseTexture1, 2);

    //setup texture slot 3
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture2->getId());
    glUniform1i(_uDiffuseTexture2, 3);

    //setup texture slot 4
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture3->getId());
    glUniform1i(_uDiffuseTexture3, 4);

    //setup texture slot 5
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture4->getId());
    glUniform1i(_uDiffuseTexture4, 5);
}
