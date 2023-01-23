//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D 		diffuseTexture;
uniform vec3			cameraPos;
uniform vec3    		lightPos0;


in		vec3			vs_position;
in		vec3			vs_normal;
in 		vec2 			texCoord;

out 	vec4 			fragment_color;

void main( void ) {
	//Ambient light
	vec3 ambientLight = vec3(0.1f, 0.1f, 0.1f);
	
	//Diffuse light
	vec3 posToLightDirVec = normalize(vs_position + lightPos0);
	vec3 diffuseColor = vec3(1.f,1.f,1.f); // color of the light
	float diffuse = clamp(dot(posToLightDirVec, vs_normal), 0,1);

	//Specualar (power of R * V to the shininess * light Color * specular color)
	vec3 lightToPosDirVec = normalize(lightPos0 - vs_position);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 posToViewDirVec = normalize(vs_position - cameraPos);
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 30);
	vec3 specularFinal = vec3(1.f, 1.f, 1.f) * specularConstant;

	//Attenuation
	float attenuation = diffuse / (.1f + .1f * distance(vs_position, lightPos0) + .1f * (distance(vs_position, lightPos0) * distance(vs_position, lightPos0)));
	vec3 diffuseFinal = diffuseColor * attenuation;

	fragment_color = texture(diffuseTexture,texCoord) * 
	(vec4(ambientLight, 1.f) + vec4(diffuseFinal,1.f) + vec4(specularFinal, 1.f));
}
