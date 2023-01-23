//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D		heightMap;
uniform sampler2D 		splatMap;
uniform sampler2D		diffuseTexture1;
uniform sampler2D		diffuseTexture2;
uniform sampler2D		diffuseTexture3;
uniform sampler2D		diffuseTexture4;

uniform vec3			cameraPos;
uniform vec3    		lightPos0;

in		vec3			vs_position;
in		vec3			vs_normal;
in 		vec2 			texCoord;

out 	vec4 			fragment_color;

void main( void ) {
	//Ambient light (turned into directional light)
	vec3 ambientLight = vec3(0.5f, 0.5f, 0.5f);
	vec3 directionalVector = vec3(1.f,1.f,1.f);
	float directionalDiffuse = clamp(dot(directionalVector, vs_normal),0,1);
	vec3 directionalFinal = directionalDiffuse * ambientLight;
	
	//Diffuse light
	vec3 posToLightDirVec = normalize(lightPos0 - vs_position);
	vec3 diffuseColor = vec3(1.f,1.f,1.f); // color of the light
	float diffuse = clamp(dot(posToLightDirVec, vs_normal), 0,1);

	//Specualar (power of R * V to the shininess * light Color * specular color)
	vec3 lightToPosDirVec = normalize(vs_position - lightPos0);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 posToViewDirVec = normalize(vec3(cameraPos - vs_position));
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 100);
	vec3 specularFinal = vec3(1.f, 1.f, 1.f) * specularConstant;

	//Attenuation
	float attenuation = diffuse / (1.f + .22f * distance(lightPos0, vs_position) + .02f * (distance(vs_position, lightPos0) * distance(vs_position, lightPos0)));
	vec3 diffuseFinal = diffuseColor * attenuation;

	vec4 splatMapTexture = texture(splatMap, texCoord);
	vec4 diffuse1 = texture(diffuseTexture1, texCoord);
	vec4 diffuse2 = texture(diffuseTexture2, texCoord);
	vec4 diffuse3 = texture(diffuseTexture3, texCoord);
	vec4 diffuse4 = texture(diffuseTexture4, texCoord);

	//fragment_color = texture(splatMap, texCoord);
	fragment_color = (diffuse1 * splatMapTexture.r) + (diffuse2 * splatMapTexture.g) + (diffuse3 * splatMapTexture.b) + (diffuse4 * splatMapTexture.a);
	fragment_color *= (vec4(directionalFinal, 1.f) + vec4(diffuseFinal,1.f) + vec4(specularFinal, 1.f));
	//fragment_color = vec4(vs_normal,1.f);

}
