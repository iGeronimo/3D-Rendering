#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <map>
#include <string>
#include <GL/glew.h>

class Terrain
{
public:
	static Terrain* load(const std::string& pTexturePath);

	GLuint getId();

protected:
	Terrain();
	virtual ~Terrain();

	static Terrain* _loadFromFile(const std::string pTexturePath);

	//OpenGL id for texture buffer
	GLuint _id;
};

#endif // TERRAIN_HPP

