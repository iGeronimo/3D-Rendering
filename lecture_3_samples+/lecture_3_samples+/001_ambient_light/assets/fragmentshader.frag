#version 330

uniform vec3 ambientLightColor;
uniform vec3 diffuseColor;

out vec4 sColor;

void main (void) {
    vec3 ambientTerm = ambientLightColor * diffuseColor;
    sColor = vec4 (ambientTerm, 1);
}
