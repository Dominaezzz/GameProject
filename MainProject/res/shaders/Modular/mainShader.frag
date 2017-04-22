#version 400

out vec4 FragColor;

void calculateTotalLighting(out vec4 diffuseLight, out vec4 specularLight);
vec4 calculateAmbientColor();
vec4 calculateDiffuseColor();
vec4 calculateSpecularColor();
vec4 calculateEmissiveColor();

void main(){
	vec4 diffuseColor = calculateDiffuseColor();
	if(diffuseColor.w == 0.0)discard;
	
	vec4 ambientLighting = vec4(1.0);
	vec4 diffuseLighting = vec4(0.0);
	vec4 specularLighting = vec4(0.0);
	
	calculateTotalLighting(diffuseLighting, specularLighting);
	
	vec4 ambient = calculateAmbientColor() * ambientLighting;
	vec4 diffuse = diffuseColor * diffuseLighting;
	vec4 specular = calculateSpecularColor() * specularLighting;
	
	FragColor = ambient + diffuse + specular; calculateEmissiveColor();
}
