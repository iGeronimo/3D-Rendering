#include <windows.h>		//master include for windows applications
#include <GL/gl.h>			//basic opengl header, uses definitions from windows.h
#include <SFML/Window.hpp>

/**
 * Basic demo of rendering a triangle in OpenGL through
 * the DEPRECATED fixed function pipeline.
 */

int main () {
    //Open SFML Window == Valid OpenGL Context
    sf::Window window(sf::VideoMode (800, 600), "OpenGL Window");
    window.setVerticalSyncEnabled( true );

    //background color
    glClearColor(0,0,0,0);

    //enter main render loop
    while (window.isOpen()) {

        //clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        //start rendering triangles sending instructions
        //to the GPU one at a time
        glBegin(GL_TRIANGLES);
            glColor3f(1,0,0);
            glVertex3f(-0.5f, -0.5f, 0);
            glColor3f(0,1,0);
            glVertex3f(0.5f , -0.5f, 0);
            glColor3f(0,0,1);
            glVertex3f(-0.5f, 0.5f, 0);
        glEnd();

        //swap current framebuffer to display
        window.display();

        //empty the event queue
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::Resized) glViewport(0, 0, event.size.width, event.size.height);
        }

    }

    return 0;
}
