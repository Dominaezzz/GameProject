#version 400

struct BaseLight
{
	vec3 color;
	float intensity;
};
struct DirectionalLight
{
	BaseLight baseLight;
	vec3 direction;
};

varying vec3 v_position;
varying vec2 v_textureCoordinates;
varying vec3 v_normal;
varying vec4 v_color;

uniform vec3 cameraPosition;
uniform sampler2D texture;

uniform float shininess;
uniform float shininessStrength;

uniform DirectionalLight directionalLight;

vec4 calculateLight(BaseLight baseLight, vec3 direction, vec3 normal)
{
	float diffuseFactor = dot(normal, -direction);
	
	vec4 diffuseColor = vec4(0.0);
	vec4 specularColor = vec4(0.0);
	
	if(diffuseFactor > 0)
	{
		diffuseColor = vec4(baseLight.color, 1.0) * baseLight.intensity * diffuseFactor;
		
		vec3 toEye = normalize(cameraPosition - v_position);
		vec3 reflectDirection = normalize(reflect(direction, normal));
		
		float specularFactor = dot(toEye, reflectDirection);
		
		if(specularFactor > 0)
		{
			specularColor = vec4(baseLight.color, 1.0) * shininess * pow(specularFactor, shininessStrength);
		}
	}
	
	return diffuseColor + specularColor;
}

vec4 calcDirectionalLight(DirectionalLight directionalLight, vec3 normal)
{
	return calculateLight(directionalLight.baseLight, directionalLight.direction, normal);
}


void main(){
	
	vec3 normal = normalize(v_normal);
	
	gl_FragColor = texture(texture, v_textureCoordinates) * calcDirectionalLight(directionalLight, normal);
}
