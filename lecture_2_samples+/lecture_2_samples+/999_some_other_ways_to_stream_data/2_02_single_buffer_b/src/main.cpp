#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cmath>

using namespace std;

#include "ShaderUtil.h"


const GLfloat dataBuffer[] = {
    //1 vertex element 3 floats, 1 color element 3 floats
    0.75f ,  -0.75f, 0, 1,0,0,
    0.75f ,  0.75f , 0, 0,1,0,
    -0.75f,  -0.75f, 0, 0,0,1
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

    while (window.isOpen()) {
		glClear( GL_COLOR_BUFFER_BIT );

        GLfloat offset[] = {0.25f * cos (clock()/500.0f), 0.25f * sin (clock()/500.0f)};
        glUniform2fv (offsetIndex, 1, offset);

        glBindBuffer (GL_ARRAY_BUFFER, dataBufferId);

		glEnableVertexAttribArray(vertexIndex);
		glEnableVertexAttribArray(colorIndex);

        //you would expect a stride of 3 here (at least I did), but in fact when specifying a stride,
        //we have to specify the WHOLE size of of "element"
		glVertexAttribPointer(vertexIndex, 3, GL_FLOAT, GL_FALSE, 6 * sizeof (GLfloat), (void*)0);
		//so same stride here (in that way it makes sense), and specify a correct offset as well
		glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, 6 * sizeof (GLfloat), (void*)(3 * sizeof (GLfloat)));

        glDrawArrays( GL_TRIANGLES, 0, 3);

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

