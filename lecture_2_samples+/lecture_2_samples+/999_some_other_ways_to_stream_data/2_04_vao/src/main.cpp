#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cmath>

using namespace std;

#include "ShaderUtil.h"

const GLfloat dataBuffer[] = {
    //1 vertex element 3 floats
    0.75f ,  -0.75f, 0,
    0.75f ,  0.75f , 0,
    -0.75f,  -0.75f, 0,
    //1 color element 3 floats
    1,0,0,
    0,1,0,
    0,0,1
};

int main () {
    sf::Window window(sf::VideoMode (800, 600), "Wawsom!");
    window.setVerticalSyncEnabled( true );

    cout << "Initializing GlEW..." << endl;
    bool glewInitResult = (glewInit() == GLEW_OK);
    cout << "GlEW Initialized:" << glewInitResult  << endl;

    GLuint programID = ShaderUtil::createProgam("vertexshader.vs", "fragmentshader.fs");
    glUseProgram (programID);

    //first load all data
    GLuint dataBufferId;
    glGenBuffers (1, &dataBufferId);
    glBindBuffer (GL_ARRAY_BUFFER, dataBufferId);
    glBufferData (GL_ARRAY_BUFFER, sizeof (dataBuffer), dataBuffer, GL_STATIC_DRAW);
    glBindBuffer (GL_ARRAY_BUFFER, 0);

    glClearColor(0,0,0,1);
    GLint vertexIndex = glGetAttribLocation(programID, "vertex");
    GLint colorIndex = glGetAttribLocation(programID, "color");
    GLint offsetIndex = glGetUniformLocation(programID, "offset");

	//a vao wraps all the setup we did before in a single reference called a vertexarray:
    GLuint dataVao;
    glGenVertexArrays (1, &dataVao);
    glBindVertexArray (dataVao);

        glBindBuffer (GL_ARRAY_BUFFER, dataBufferId);

		glEnableVertexAttribArray(vertexIndex);
		glEnableVertexAttribArray(colorIndex);

		glVertexAttribPointer(vertexIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*)(3 * 3 * sizeof (GLfloat)));

        //http://stackoverflow.com/questions/12427880/is-it-important-to-call-gldisablevertexattribarray
		//glDisableVertexAttribArray(vertexIndex);
		//glDisableVertexAttribArray(colorIndex);

    glBindVertexArray(0);

    while (window.isOpen()) {
		glClear( GL_COLOR_BUFFER_BIT );

        GLfloat offset[] = {0.25f * cos (clock()/500.0f), 0.25f * sin (clock()/500.0f)};
        glUniform2fv (offsetIndex, 1, offset);

        glBindVertexArray (dataVao);
        glDrawArrays( GL_TRIANGLES, 0, 3);
        glBindVertexArray (0);

        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::Resized) glViewport(0, 0, event.size.width, event.size.height);
        }
    }

    return 0;
}

