//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D   heightMap;

uniform mat4        modelMatrix;
uniform	mat4 	    mvpMatrix;
uniform float       heightConstant;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec3 vs_position;
out vec3 vs_normal;
out vec2 texCoord; //make sure the texture coord is interpolated

void main( void ){
    float stepConstant = .08f;

    vec3 _vertex = vertex;
    vec4 color = texture(heightMap, uv);
    vec4 colorLeft = texture(heightMap, vec2(uv.x - stepConstant, uv.y));
    vec4 colorRight = texture(heightMap, vec2(uv.x + stepConstant, uv.y));
    vec4 colorUp = texture(heightMap, vec2(uv.x, uv.y + stepConstant));
    vec4 colorDown = texture(heightMap, vec2(uv.x, uv.y - stepConstant));

    vec3 vertexLeft = vertex;
    vertexLeft.x -= stepConstant;
    vertexLeft.y = ((colorLeft.x + colorLeft.y + colorLeft.z) / 3.f) * heightConstant;

    vec3 vertexRight = vertex;
    vertexRight.x += stepConstant;
    vertexRight.y = ((colorRight.x + colorRight.y + colorRight.z) / 3.f) * heightConstant;

    vec3 vertexDown = vertex;
    vertexDown.z -= stepConstant;
    vertexDown.y = ((colorDown.x + colorDown.y + colorDown.z) / 3.f) * heightConstant;

    vec3 vertexUp = vertex;
    vertexUp.z += stepConstant;
    vertexUp.y = ((colorUp.x + colorUp.y + colorUp.z) / 3.f) * heightConstant;

    //vertexLeft *= mat3(modelMatrix);
    //vertexRight *= mat3(modelMatrix);
    //vertexUp *= mat3(modelMatrix);
    //vertexDown *= mat3(modelMatrix);

    vec3 vToCross = vertexDown - vertexUp;
    vec3 hToCross = vertexRight - vertexLeft;

    _vertex.y =  ((color.x + color.y + color.z) / 3.f) * heightConstant;
    vs_position = vec4(modelMatrix * vec4(_vertex, 1.f)).xyz;
	texCoord = uv;
    vs_normal = normalize(mat3(modelMatrix) * (cross(normalize(hToCross), normalize(vToCross))));

    gl_Position = mvpMatrix * vec4(_vertex, 1.f);
}
