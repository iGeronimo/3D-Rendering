#version 330

uniform mat4 mvpMatrix;

in vec3 vertex;
in vec3 color;

out vec3 fColor;

void main (void) {
    //C = rotate * translation
    //P * V * M

    gl_Position = mvpMatrix * vec4(vertex,1);
    fColor = color;
}

