#ifndef TERRAINMATERIAL_HPP
#define TEXTUREMATERIAL_HPP

#include "mge/materials/AbstractMaterial.hpp"
#include "gl/glew.h"

class ShaderProgram;
class Texture;

class TerrainMaterial : public AbstractMaterial
{
	public:
		TerrainMaterial(Texture* pHeightMap, Texture* pSplatMap, Texture* pDiffuseTexture1, Texture* pDiffuseTexture2, Texture* pDiffuseTexture3, Texture* pDiffuseTexture4);
		virtual ~TerrainMaterial();

		virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

        void setTextureHeightMap(Texture* pDiffuseTexture);
        void setTextureSplatMap(Texture* pDiffuseTexture);

		void setDiffuseTexture1(Texture* pDiffuseTexture);
        void setDiffuseTexture2(Texture* pDiffuseTexture);
        void setDiffuseTexture3(Texture* pDiffuseTexture);
        void setDiffuseTexture4(Texture* pDiffuseTexture);

    protected:
    private:
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        //in this example we cache all identifiers for uniforms & attributes
        static GLint _uMVPMatrix;

        static GLint _uHeightMap;
        static GLint _uSplatMap;

        static GLint _uDiffuseTexture1;
        static GLint _uDiffuseTexture2;
        static GLint _uDiffuseTexture3;
        static GLint _uDiffuseTexture4;

        static GLint _aVertex;
        static GLint _aNormal;
        static GLint _aUV;

        Texture* _heightMap;
        Texture* _splatMap;

        Texture* _diffuseTexture1;
        Texture* _diffuseTexture2;
        Texture* _diffuseTexture3;
        Texture* _diffuseTexture4;

        TerrainMaterial(const TerrainMaterial&);
        TerrainMaterial& operator=(const TerrainMaterial&);
};

#endif // TerrainMaterial_HPP

