#version 330

uniform vec3 diffuseColor;
uniform vec3 ambientLightColor;
uniform vec3 directionalLightColor;

in float diffuseIntensity;
out vec4 sColor;

void main (void) {
    vec3 ambientTerm = ambientLightColor * diffuseColor;

    vec3 diffuseTerm = diffuseIntensity * directionalLightColor * diffuseColor;

    sColor = vec4 (ambientTerm + diffuseTerm , 1);
}
