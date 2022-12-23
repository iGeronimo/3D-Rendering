#include <GL/glew.h>
#include <SFML/Graphics.hpp>

#include "glm.hpp"

#include <iostream>
#include <cmath>

using namespace std;

#include "ShaderUtil.h"
#include "Mesh.hpp"

//Some changes:
//meshes are now loaded through a utility mesh class
//attribute indexes are now prefixed by a
//uniform indexes are now prefixed by u

int main () {
    //Open SFML Window == Valid OpenGL Context
    sf::RenderWindow window(sf::VideoMode (800, 600), "OpenGL Window", sf::Style::Default, sf::ContextSettings(24,0,0,3,3));
    window.setVerticalSyncEnabled(true);

    //Get some basic setting info through OpenGL
    sf::ContextSettings settings = window.getSettings();
    cout << settings.majorVersion << "." << settings.minorVersion << endl;

    //initialize glew to load all available opengl functions
    cout << "Initializing GlEW..." << endl;
    bool glewInitResult = (glewInit() == GLEW_OK);
    cout << "GlEW Initialized:" << glewInitResult  << endl;

    //create shader program (see shaderutil)
    GLuint programID = ShaderUtil::createProgam("vertexshader.vert", "fragmentshader.frag");
    glUseProgram (programID);

    glEnable (GL_DEPTH_TEST);

    Mesh mesh;
    mesh.load("sphere_80_triangles_flat.obj");

    GLuint vertexBufferId;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size()*sizeof(glm::vec3), &mesh.vertices[0], GL_STATIC_DRAW );

    /**
    //Not using the normal's
    GLuint normalBufferId;
    glGenBuffers(1, &normalBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferId );
    glBufferData(GL_ARRAY_BUFFER, mesh.normals.size()*sizeof(glm::vec3), &mesh.normals[0], GL_STATIC_DRAW );
    /**/

    /**
    //Not using the uv's
    GLuint uvBufferId;
    glGenBuffers(1, &uvBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, uvBufferId );
    glBufferData(GL_ARRAY_BUFFER, mesh.uvs.size()*sizeof(glm::vec2), &mesh.uvs[0], GL_STATIC_DRAW );
    /**/

    glBindBuffer(GL_ARRAY_BUFFER, 0 );

    //indices
    GLint aVertexIndex = glGetAttribLocation(programID, "vertex");

    GLint uMVPIndex = glGetUniformLocation (programID, "mvpMatrix");

    GLint uDiffuseColorIndex = glGetUniformLocation (programID, "diffuseColor");
    GLint uAmbientLightColorIndex = glGetUniformLocation (programID, "ambientLightColor");

    //values
    glm::vec3 diffuseColor = glm::vec3 (1,1,0);
    glm::vec3 ambientLightColor = glm::vec3(0, 1, 1) * 1.0f;

    //matrices

    while (window.isOpen()) {
        sf::Vector2u windowSize = window.getSize();
        glm::mat4 modelMatrix       = glm::rotate (clock()/1000.0f, glm::vec3(0.7,0.7,0.7));
        glm::mat4 cameraMatrix      = glm::translate (glm::vec3(0,0,5));
        glm::mat4 projectionMatrix  = glm::perspective (glm::radians(60.0f), windowSize.x / (float)windowSize.y, 0.1f, 1000.0f);

        glm::mat4 mvp = projectionMatrix * glm::inverse(cameraMatrix) * modelMatrix;

        //glm::vec3 ambientLightColor = glm::vec3(0, 1, 1) * abs(sin (clock()/100.0f));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniformMatrix4fv (uMVPIndex, 1, GL_FALSE, glm::value_ptr(mvp));
        glUniform3fv (uDiffuseColorIndex, 1, glm::value_ptr(diffuseColor));
        glUniform3fv (uAmbientLightColorIndex, 1, glm::value_ptr(ambientLightColor));

		glEnableVertexAttribArray(aVertexIndex);

        glBindBuffer (GL_ARRAY_BUFFER, vertexBufferId);
		glVertexAttribPointer(aVertexIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glDrawArrays ( GL_TRIANGLES, 0, mesh.vertices.size());

		glDisableVertexAttribArray(aVertexIndex);

        //display it
        window.display();

        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

    }

    return 0;
}
