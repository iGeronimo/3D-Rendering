#ifndef MESH_H
#define MESH_H
#include <vector>
#include <cstring>
#include <string>
#include "glm.hpp"

/**
 * A mesh represents an .OBJ file.
 */
class Mesh
{
	public:
		Mesh();
		virtual ~Mesh();

		void load(std::string pFileName);

	    //the actual data
		std::vector<glm::vec3> vertices;       //vec3 with 3d coords for all vertices
		std::vector<glm::vec3> normals;        //vec3 with 3d normal data
		std::vector<glm::vec2> uvs;            //vec2 for uv
};

#endif // MESH_H
