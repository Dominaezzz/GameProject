#version 440

struct Attenuation
{
	float constant;
	float linear;
	float quadratic;
};
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

const int MAX_DIRECTIONAL_LIGHTS = 2;
const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 4;

in vec3 v_position;

layout (std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec3 cameraPosition;
};

layout (std140, binding = 1) uniform Lights
{
	DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
	PointLight pointLights[MAX_POINT_LIGHTS];
	SpotLight spotLights[MAX_SPOT_LIGHTS];
};

//uniform vec3 cameraPosition;

uniform float reflectivity;
uniform float shininess;

vec3 vertexNormal = vec3(0.0);
vec3 toEye = vec3(0.0);

vec3 calculateNormal();
void calculateDirectionalLight(DirectionalLight directionalLight, out vec3 diffuseColor, out vec3 specularColor);
void calculatePointLight(PointLight pointLight, out vec3 diffuseColor, out vec3 specularColor);
void calculateSpotLight(SpotLight spotLight, out vec3 diffuseColor, out vec3 specularColor);

void calculateTotalLighting(out vec4 diffuseLight, out vec4 specularLight)
{
	vertexNormal = calculateNormal();
	toEye = normalize(cameraPosition - v_position);
	
	vec3 totalDiffuse = vec3(0.0);
	vec3 totalSpecular = vec3(0.0);
	
	for(int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
	{
		vec3 diffuse = vec3(0.0);
		vec3 specular = vec3(0.0);
		calculateDirectionalLight(directionalLights[i], diffuse, specular);
		totalDiffuse += diffuse;
		totalSpecular += specular;
	}
	
	for(int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		vec3 diffuse = vec3(0.0);
		vec3 specular = vec3(0.0);
		calculatePointLight(pointLights[i], diffuse, specular);
		totalDiffuse += diffuse;
		totalSpecular += specular;
	}
	
	for(int i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		vec3 diffuse = vec3(0.0);
		vec3 specular = vec3(0.0);
		calculateSpotLight(spotLights[i], diffuse, specular);
		totalDiffuse += diffuse;
		totalSpecular += specular;
	}
	
	diffuseLight = vec4(totalDiffuse, 1.0);
	specularLight = vec4(totalSpecular, 1.0);
}


float calculateAttenuation(Attenuation attenuation, float radius)
{
	float value = attenuation.constant + (attenuation.linear * radius) + (attenuation.quadratic * (radius * radius));
	return max(value, 1.0);
}

void calculateLight(BaseLight baseLight, vec3 direction, out vec3 diffuseColor, out vec3 specularColor)
{
	if(baseLight.intensity <= 0.0)return;

	float diffuseFactor = dot(vertexNormal, -direction);
	
	if(diffuseFactor > 0.0)
	{
		diffuseColor = baseLight.color * baseLight.intensity * diffuseFactor;
		
		vec3 reflectDirection = normalize(reflect(direction, vertexNormal));
		
		float specularFactor = dot(toEye, reflectDirection);
		
		if(specularFactor > 0.0)
		{
			specularColor = baseLight.color * reflectivity * pow(specularFactor, shininess);
		}
	}
}

void calculateDirectionalLight(DirectionalLight directionalLight, out vec3 diffuseColor, out vec3 specularColor)
{
	calculateLight(directionalLight.baseLight, directionalLight.direction, diffuseColor, specularColor);
}

void calculatePointLight(PointLight pointLight, out vec3 diffuseColor, out vec3 specularColor)
{
	if(pointLight.baseLight.intensity > 0.0)
	{
		vec3 lightDirection = v_position - pointLight.position;
		float distanceToLight = length(lightDirection);
		
		calculateLight(pointLight.baseLight, (lightDirection / distanceToLight), diffuseColor, specularColor);
		
		float attenuation = 1.0 / calculateAttenuation(pointLight.attenuation, distanceToLight);
		
		if(distanceToLight > pointLight.radius) attenuation = 0;
		
		diffuseColor *= attenuation;
		specularColor *= attenuation;
	}
}

void calculateSpotLight(SpotLight spotLight, out vec3 diffuseColor, out vec3 specularColor)
{
	if(spotLight.pointLight.baseLight.intensity > 0.0)
	{
		vec3 lightDirection = normalize(v_position - spotLight.pointLight.position);
		float spotFactor = dot(lightDirection, spotLight.direction);
		
		spotFactor = max(spotFactor, spotLight.cutOff);

		calculatePointLight(spotLight.pointLight, diffuseColor, specularColor);
		
		float coneAttenuation = 1.0 - ((1.0 - spotFactor) / (1.0 - spotLight.cutOff));

		diffuseColor *= coneAttenuation;
		specularColor *= coneAttenuation;
	}
}
