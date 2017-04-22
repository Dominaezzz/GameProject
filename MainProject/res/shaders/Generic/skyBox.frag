#version 400

in vec3 v_textureCoords;

out vec4 FragColor;

uniform samplerCube cubeMap;

void main(){
	FragColor = texture(cubeMap, v_textureCoords);
}
