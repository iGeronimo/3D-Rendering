#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
#include "mge/core/Terrain.hpp"

Terrain::Terrain() : _id() {
    glGenTextures(1, &_id);
}

Terrain::~Terrain()
{
    glDeleteTextures(1, &_id);
}

GLuint Terrain::getId() {
    return _id;
}

// importer for textures
Terrain* Terrain::load(const std::string& pFilename)
{
    // load from file and store in cache
    sf::Image image;
    if (image.loadFromFile(pFilename)) {
        //normal image 0,0 is top left, but opengl considers 0,0 to be bottom left, so we flip the image internally
        image.flipVertically();
        //create a wrapper for the id (texture is nothing more than that) and
        //load corresponding data into opengl using this id
        Terrain* terrain = new Terrain();
        glBindTexture(GL_TEXTURE_2D, terrain->getId());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        return terrain;
    }
    else {
        return nullptr;
    }
}
