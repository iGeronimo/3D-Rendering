#include "Mesh.hpp"
#include <iostream>
#include <map>
#include <string>
#include <fstream>

using namespace std;

Mesh::Mesh()
{
	//ctor
}

Mesh::~Mesh()
{
	//dtor
}

void Mesh::load(string pFileName)
{
    cout << "Loading " << pFileName << "..." << endl;

	ifstream file (pFileName, ios::in);

	if( file.is_open() ){
        //these three vectors will contains data as taken from the obj file
        //in the order it is encountered in the object file
		vector<glm::vec3> verticesFromFile;
		vector<glm::vec3> normalsFromFile;
		vector<glm::vec2> uvsFromFile;

		string line; // to store each line in
		while(getline(file,line)) {
            //get the type of line so that we know how to process it
			char cmd[10];
			sscanf ( line.c_str(), "%10s", cmd );

			//vertex
			if ( strcmp ( cmd, "v" ) == 0 ) {
				glm::vec3 vertex;
				sscanf(line.c_str(), "%10s %f %f %f ", cmd, &vertex.x, &vertex.y, &vertex.z );
				verticesFromFile.push_back( vertex );
            //normal
			} else if ( strcmp ( cmd, "vn" ) == 0 ) {
				glm::vec3 normal;
				sscanf(line.c_str(), "%10s %f %f %f ", cmd, &normal.x, &normal.y, &normal.z );
				normalsFromFile.push_back( normal );
            //uv
			} else if ( strcmp ( cmd, "vt" ) == 0 ) {
				glm::vec2 uv;
				sscanf(line.c_str(), "%10s %f %f ", cmd, &uv.x, &uv.y );
				uvsFromFile.push_back( uv );
            //triangle spec
			} else if ( strcmp ( cmd, "f" ) == 0 ) {
			    //an f lines looks like
			    //f 2/1/1 1/2/1 3/3/1
			    //in other words
			    //f v1/u1/n1 v2/u2/n2 v3/u3/n3
				glm::ivec3 vertexIndex;
				glm::ivec3 normalIndex;
				glm::ivec3 uvIndex;
			    int count = sscanf(line.c_str(), "%10s %d/%d/%d %d/%d/%d %d/%d/%d", cmd, &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );

                //Have we read exactly 10 elements?
				if ( count == 10 ) {
                    //process 3 triplets, one for each vertex (which is first element of the triplet)
					for ( int i = 0; i < 3; ++i ) {
							//and store the corresponding vertex/normal/uv values into our own buffers
							//note the -1 is required since all values in the f triplets in the .obj file
							//are 1 based, but our vectors are 0 based
							vertices.push_back( verticesFromFile[ vertexIndex[i]-1 ]);
							normals.push_back(  normalsFromFile[ normalIndex[i]-1 ] );
							uvs.push_back(      uvsFromFile[ uvIndex[i]-1 ]         );
					}
				} else {
				    //If we read a different amount, something is wrong
					cout << "Error reading obj, needing v,vn,vt" << endl;
				}
			}
		}

		file.close();
		cout << "Mesh loaded, " << (vertices.size()/3) << " triangles." << endl;
	} else {
		cout << "Could not read " << pFileName << endl;
	}
}





