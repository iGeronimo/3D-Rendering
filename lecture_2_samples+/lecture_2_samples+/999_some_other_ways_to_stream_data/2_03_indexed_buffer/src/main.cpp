#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cmath>

using namespace std;

#include "ShaderUtil.h"

/**
 * This example demonstrates index buffers
 */

const float vertices[] = {
    0.4, 0.2, 0.0,
    0.6, 0.2, 0.0,

    0.2, 0.4, 0.0,
    0.4, 0.4, 0.0,
    0.6, 0.4, 0.0,
    0.8, 0.4, 0.0,

    0.2, 0.6, 0.0,
    0.4, 0.6, 0.0,
    0.6, 0.6, 0.0,
    0.8, 0.6, 0.0,

    0.4, 0.8, 0.0,
    0.6, 0.8, 0.0
};

const GLfloat colors[] = {
	1,0,0,
	0,1,0,
	0,0,1,
	1,0,0,
	0,1,0,
	0,0,1,
	0,1,0,
	0,0,1,
	0,1,0,
	0,0,1,
	0,1,0,
	0,0,1
};

const GLuint indexes[] = {
    0,3,1,1,3,4,2,6,3,3,6,7,3,7,4,4,7,8,5,4,8,5,8,9,7,8,10,10,11,8
};

int main () {
    sf::Window window(sf::VideoMode (800, 600), "Wawsom!");
    window.setVerticalSyncEnabled( true );

    cout << "Initializing GlEW..." << endl;
    bool glewInitResult = (glewInit() == GLEW_OK);
    cout << "GlEW Initialized:" << glewInitResult  << endl;

    GLuint programID = ShaderUtil::createProgam("vertexshader.vs", "fragmentshader.fs");
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

    GLuint indexBufferId;
    glGenBuffers (1, &indexBufferId);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (indexes), indexes, GL_STATIC_DRAW);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);

    glClearColor(0,0,0,1);
    GLint vertexIndex = glGetAttribLocation(programID, "vertex");
    GLint colorIndex = glGetAttribLocation(programID, "color");
    GLint offsetIndex = glGetUniformLocation(programID, "offset");

    while (window.isOpen()) {
		glClear( GL_COLOR_BUFFER_BIT );

        GLfloat offset[] = {-0.5, -0.5};
        glUniform2fv (offsetIndex, 1, offset);

		glEnableVertexAttribArray(vertexIndex);
        glBindBuffer (GL_ARRAY_BUFFER, vertexBufferId);
		glVertexAttribPointer(vertexIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(colorIndex);
		glBindBuffer (GL_ARRAY_BUFFER, colorBufferId);
		glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

        glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, (void*)0);

		glDisableVertexAttribArray(vertexIndex);
		glDisableVertexAttribArray(colorIndex);

        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::Resized) glViewport(0, 0, event.size.width, event.size.height);
        }
    }

    return 0;
}

