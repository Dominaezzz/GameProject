#include "light_system.h"

LightSystem::LightSystem(World * world) : System(world)
{
	lightsBuffer.bindBase(1);
}

void LightSystem::setBaseLight(Lights::BaseLight& baseLight, const Light& light)
{
	baseLight.color.x = light.color.r;
	baseLight.color.y = light.color.g;
	baseLight.color.z = light.color.b;
	baseLight.intensity = light.intensity;
}

void LightSystem::setDirectionalLight(Lights::DirectionalLight& directionalLight, const Transform& transform, const Light& light)
{
	setBaseLight(directionalLight.baseLight, light);
	directionalLight.direction = transform.getTransform().transform(Vector3(0, 1, 0), 0).normalize();
}

void LightSystem::setPointLight(Lights::PointLight& pointLight, const Transform& transform, const Light& light)
{
	setBaseLight(pointLight.baseLight, light);
	pointLight.attenuation = light.attenuation;
	pointLight.position = transform.getWorldPosition();
	pointLight.radius = light.range;
}

void LightSystem::setSpotLight(Lights::SpotLight& spotLight, const Transform& transform, const Light& light)
{
	setPointLight(spotLight.pointLight, transform, light);
	spotLight.direction = transform.getTransform().transform(Vector3(0, 1, 0), 0).normalize();
	spotLight.cutOff = cos(light.coneAngle);
}

void LightSystem::render()
{
	size_t directionalIndex = 0;
	size_t pointIndex = 0;
	size_t spotIndex = 0;

	for (size_t i = 0; i < lightNodes.size(); ++i)
	{
		auto transform = lightNodes[i].get<Transform>();
		auto light = lightNodes[i].get<Light>();

		switch (light->lightType)
		{
			case LightType::Directional:
				setDirectionalLight(lights.directionalLights[directionalIndex++], *transform, *light);
				break;
			case LightType::Point:
				setPointLight(lights.pointLights[pointIndex++], *transform, *light);
				break;
			case LightType::Spot:
				setSpotLight(lights.spotLights[spotIndex++], *transform, *light);
				break;
			default: ;
		}
	}
	lightsBuffer.setSubData(&lights, 0, sizeof(Lights));
}

void LightSystem::onGameObjectEdited(GameObject* gameObject)
{
	lightNodes.reportChange(gameObject);
}
