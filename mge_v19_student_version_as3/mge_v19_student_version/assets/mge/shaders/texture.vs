//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform mat4    modelMatrix;
uniform	mat4 	mvpMatrix;


in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec3 vs_position;
out vec3 vs_normal;
out vec2 texCoord; //make sure the texture coord is interpolated

void main( void ){
    vs_position = vec4(modelMatrix * vec4(vertex, 1.f)).xyz;
	texCoord = uv;
    vs_normal = mat3(modelMatrix) * normal;

    gl_Position = mvpMatrix * vec4(vertex, 1.f);
}
