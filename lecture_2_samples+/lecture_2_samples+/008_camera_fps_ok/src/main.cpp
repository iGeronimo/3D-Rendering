#include <GL/glew.h>
#include <SFML/Graphics.hpp>

#include "glm.hpp"

#include <iostream>
#include <cmath>

using namespace std;

#include "ShaderUtil.h"


const float vertices[] = {
    //front triangle 1
    -1,	1,	1,
    -1,	-1,	1,
    1,	1,	1,

    //front triangle 2
    -1,	-1,	1,
    1,	-1,	1,
    1,	1,	1,

    //back triangle 1
    -1,	1,	-1,
    1,	1,	-1,
    -1,	-1,	-1,

    //back triangle 2
    -1,	-1,	-1,
    1,	1,	-1,
    1,	-1,	-1,

    //top triangle 1
    -1,	1,	1,
    1,	1,	1,
    1,	1,	-1,

    //top triangle 2
    1,	1,	-1,
    -1,	1,	-1,
    -1,	1,	1,

    //bottom triangle 1
    -1,	-1,	1,
    1,	-1,	-1,
    1,	-1,	1,

    //bottom triangle 2
    1,	-1,	-1,
    -1,	-1,	1,
    -1,	-1,	-1,

    //right triangle 1
    1,	-1,	1,
    1,	-1,	-1,
    1,	1,	1,

    //right triangle 2
    1,	-1,	-1,
    1,	1,	-1,
    1,	1,	1,

    //left triangle 1
    -1,	-1,	1,
    -1,	1,	1,
    -1,	-1,	-1,

    //left triangle 2
    -1,	-1,	-1,
    -1,	1,	1,
    -1,	1,	-1
};

const float colors[] = {
    //front triangle 1
    0,	1,	1,
    0,	0,	1,
    1,	1,	1,

    //front triangle 2
    0,	0,	1,
    1,	0,	1,
    1,	1,	1,

    //back triangle 1
    0,	1,	0,
    1,	1,	0,
    0,	0,	0,

    //back triangle 2
    0,	0,	0,
    1,	1,	0,
    1,	0,	0,

    //top triangle 1
    0,	1,	1,
    1,	1,	1,
    1,	1,	0,

    //top triangle 2
    1,	1,	0,
    0,	1,	0,
    0,	1,	1,

    //bottom triangle 1
    0,	0,	1,
    1,	0,	0,
    1,	0,	1,

    //bottom triangle 2
    1,	0,	0,
    0,	0,	1,
    0,	0,	0,

    //right triangle 1
    1,	0,	1,
    1,	0,	0,
    1,	1,	1,

    //right triangle 2
    1,	0,	0,
    1,	1,	0,
    1,	1,	1,

    //left triangle 1
    0,	0,	1,
    0,	1,	1,
    0,	0,	0,

    //left triangle 2
    0,	0,	0,
    0,	1,	1,
    0,	1,	0
};


int main () {
    //Open SFML Window == Valid OpenGL Context
    sf::RenderWindow window(sf::VideoMode (800, 600), "Wawsom!", sf::Style::Default, sf::ContextSettings(24,0,0,3,3));
    window.setVerticalSyncEnabled( true );

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
    glEnable (GL_CULL_FACE);

    GLuint vertexBufferId;
    glGenBuffers (1, &vertexBufferId);
    glBindBuffer (GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer (GL_ARRAY_BUFFER, 0);

    GLuint colorBufferId;
    glGenBuffers (1, &colorBufferId);
    glBindBuffer (GL_ARRAY_BUFFER, colorBufferId);
    glBufferData (GL_ARRAY_BUFFER, sizeof (colors), colors, GL_STATIC_DRAW);
    glBindBuffer (GL_ARRAY_BUFFER, 0);

    GLint vertexIndex = glGetAttribLocation(programID, "vertex");
    GLint colorIndex = glGetAttribLocation(programID, "color");

    glm::mat4 cameraMatrix(glm::translate(glm::mat4(1.f), glm::vec3(0,0,5)));

    while (window.isOpen()) {

        //translate camera
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            //notice the -!
            cameraMatrix = cameraMatrix * glm::translate(glm::mat4(1.f), glm::vec3(0,0,-0.1f));
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            cameraMatrix = cameraMatrix * glm::translate (glm::mat4(1.f), glm::vec3(0,0,0.1f)) ;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            cameraMatrix = cameraMatrix * glm::translate (glm::mat4(1.f), glm::vec3(-0.1f,0,0));
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            cameraMatrix = cameraMatrix * glm::translate (glm::mat4(1.f), glm::vec3(0.1f,0,0)) ;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            cameraMatrix = cameraMatrix * glm::translate (glm::mat4(1.f), glm::vec3(0,0.1f,0)) ;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            cameraMatrix = cameraMatrix * glm::translate (glm::mat4(1.f), glm::vec3(0,-0.1f,0)) ;
        }

        //rotate camera
        glm::vec3 cameraPosition = glm::vec3(cameraMatrix[3]);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            //one way to rotate around the camera world y axis
            cameraMatrix = glm::rotate (cameraMatrix, -0.01f, glm::vec3(0,1,0)) * glm::translate(glm::mat4(1.f), cameraPosition) * glm::translate (glm::mat4(1.f), -cameraPosition) * cameraMatrix;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            //another way to rotate around the center world y axis
            cameraMatrix[3] = glm::vec4(0,0,0,1);
            cameraMatrix = glm::rotate (cameraMatrix, -0.01f, glm::vec3(0,1,0)) * cameraMatrix;
            cameraMatrix[3] = glm::vec4 (cameraPosition,1);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            //go up
            cameraMatrix = glm::rotate (cameraMatrix, 0.01f, glm::vec3(1,0,0)) * cameraMatrix;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            //go down using the old way, same as post multiplying this rotation matrix
            cameraMatrix = glm::rotate (cameraMatrix, -0.01f, glm::vec3(1,0,0));
        }

        //(re)constructing an oriented positioned camera matrix out of a position and a quaternion is also possible
		//but we'll get there in another lecture

        glm::mat4 viewMatrix = glm::inverse(cameraMatrix);

        sf::Vector2u windowSize = window.getSize();
        glm::mat4 projectionMatrix = glm::perspective (glm::radians(60.0f), windowSize.x / (float)windowSize.y, 0.1f, 1000.0f);

        glm::mat4 vp = projectionMatrix * viewMatrix;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnableVertexAttribArray(vertexIndex);
		glEnableVertexAttribArray(colorIndex);
        glBindBuffer (GL_ARRAY_BUFFER, vertexBufferId);
		glVertexAttribPointer(vertexIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glBindBuffer (GL_ARRAY_BUFFER, colorBufferId);
		glVertexAttribPointer(colorIndex,   3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glm::mat4 modelMatrix1 (glm::scale (glm::mat4(1.f), glm::vec3(3,0.01f,3)));
        glUniformMatrix4fv (glGetUniformLocation (programID, "mvpMatrix"), 1, GL_FALSE, glm::value_ptr(vp*modelMatrix1));
        glDrawArrays( GL_TRIANGLES, 0, 36);

        glm::mat4 modelMatrix2    (glm::translate (glm::mat4(1.f), glm::vec3(0,1,0)));
        glUniformMatrix4fv (glGetUniformLocation (programID, "mvpMatrix"), 1, GL_FALSE, glm::value_ptr(vp*modelMatrix2));
        glDrawArrays( GL_TRIANGLES, 0, 36);

		glDisableVertexAttribArray(vertexIndex);
		glDisableVertexAttribArray(colorIndex);

        //display it
        window.display();

        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::Resized) glViewport(0, 0, event.size.width, event.size.height);
        }

    }

    return 0;
}
