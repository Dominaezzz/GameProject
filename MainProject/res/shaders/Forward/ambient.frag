#version 400

varying vec2 v_textureCoordinates;

uniform vec4 ambientColor;
uniform sampler2D texture;


void main(){
	vec4 totalColor = ambientColor;
	
	gl_FragColor = texture(texture, v_textureCoordinates) * totalColor;
}
