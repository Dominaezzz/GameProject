#version 400

varying vec3 v_position;
varying vec2 v_textureCoordinates;
varying vec3 v_normal;
varying vec4 v_color;

uniform vec3 cameraPosition;
uniform sampler2D texture;

uniform float shininess;
uniform float shininessStrength;

struct BaseLight
{
	vec3 color;
	float intensity;
};
struct Attenuation
{
	float constant;
	float linear;
	float exponent;
};
struct PointLight
{
	BaseLight baseLight;
	Attenuation attenuation;
	vec3 position;
	float radius;
};
struct SpotLight
{
	PointLight pointLight;
	vec3 direction;
	float cutOff;
};

uniform SpotLight spotLight;

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

vec4 calcPointLight(PointLight pointLight, vec3 normal)
{
	vec3 lightDirection = v_position - pointLight.position;
	float distanceToLight = length(lightDirection);
	if(distanceToLight > pointLight.radius)
	{
		return vec4(0.0);
	}
	lightDirection = normalize(lightDirection);
	
	vec4 color = calculateLight(pointLight.baseLight, lightDirection, normal);
	
	float attenuation = pointLight.attenuation.constant +
						(pointLight.attenuation.linear * distanceToLight) +
						(pointLight.attenuation.exponent * pow(distanceToLight, 2));
						
	attenuation = max(attenuation, 1.0);
	
	return color / attenuation;
}

vec4 calcSpotLight(SpotLight spotLight, vec3 normal)
{
	vec3 lightDirection = normalize(v_position - spotLight.pointLight.position);
	float spotFactor = dot(lightDirection, spotLight.direction);
	
	vec4 color = vec4(0.0);
	
	if(spotFactor > spotLight.cutOff)
	{
		color = calcPointLight(spotLight.pointLight, normal);
		color *= 1.0 - ((1.0 - spotFactor) / (1.0 - spotLight.cutOff));
	}
	
	return color;
}

void main(){
	vec3 normal = normalize(v_normal);
	
	gl_FragColor = texture(texture, v_textureCoordinates) * calcSpotLight(spotLight, normal);
}
