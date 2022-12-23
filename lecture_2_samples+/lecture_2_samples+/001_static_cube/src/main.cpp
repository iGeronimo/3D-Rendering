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
    sf::Window window(sf::VideoMode (800, 600), "OpenGL Window");
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

    /////////////////////////////////////////
    /////		THE NEW THINGS .....

    //where is the model?
    glm::mat4 modelMatrix = glm::translate(glm::vec3(0, 0, -5));
    std::cout << "Model matrix" << std::endl;
    std::cout << modelMatrix << std::endl;

    //where is the camera?
    glm::mat4 cameraMatrix = glm::mat4(1.0f);
    std::cout << "Camera matrix" << std::endl;
    std::cout << cameraMatrix << std::endl;

    //based on the camera, what is the view matrix ? (also the identity matrix in this case)
    glm::mat4 viewMatrix = glm::inverse(cameraMatrix);
    std::cout << "View matrix" << std::endl;
    std::cout << viewMatrix << std::endl;

//    std::cout << "Projection matrix" << std::endl;
 //   std::cout << projectionMatrix << std::endl;


    while (window.isOpen()) {

        //what is our camera setup?
        sf::Vector2u windowSize = window.getSize();
        glm::mat4 projectionMatrix =
            glm::perspective(glm::radians(60.0f), windowSize.x / (float)windowSize.y, 0.1f, 1000.0f);

        //what is the resulting mvp matrix?
        glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

        glClear(GL_COLOR_BUFFER_BIT);

		//two ways to say the same thing
        //glUniformMatrix4fv (glGetUniformLocation (programID, "mvpMatrix"), 1, GL_FALSE, &mvp[0][0]);
        glUniformMatrix4fv (glGetUniformLocation (programID, "mvpMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));

		/////////////////////////////////////////
		/////		THE OLD THINGS .....

		glEnableVertexAttribArray(vertexIndex);
		glEnableVertexAttribArray(colorIndex);

        glBindBuffer (GL_ARRAY_BUFFER, vertexBufferId);
		glVertexAttribPointer(vertexIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glBindBuffer (GL_ARRAY_BUFFER, colorBufferId);
		glVertexAttribPointer(colorIndex,   3, GL_FLOAT, GL_FALSE, 0, (void*)0);
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
