#include "light_system.h"

void LightNode::setGameObject(GameObject * gameObject)
{
	Node::setGameObject(gameObject);
	transform = gameObject->getComponent<Transform>();
	light = gameObject->getComponent<Light>();
}

LightSystem::LightSystem(World * world) : System(world)
{
	lightsBuffer.bindBase(1);
}

LightSystem::~LightSystem()
{
}

void LightSystem::update(float dt)
{
}

void LightSystem::setBaseLight(Lights::BaseLight& baseLight, const Light& light)
{
	baseLight.color = Vector3(light.color.r / 255.0F, light.color.g / 255.0F, light.color.b / 255.0F);
	baseLight.intensity = light.intensity;
}

void LightSystem::setDirectionalLight(Lights::DirectionalLight& directionalLight, const Transform& transform, const Light& light)
{
	setBaseLight(directionalLight.baseLight, light);
	directionalLight.direction = transform.getTransform().transform(Vector3(0, 1, 0), 0);
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
	spotLight.direction = transform.getTransform().transform(Vector3(0, 1, 0), 0);
	spotLight.cutOff = cos(light.coneAngle);
}

void LightSystem::render()
{
	size_t directionalIndex = 0;
	size_t pointIndex = 0;
	size_t spotIndex = 0;

	for (size_t i = 0; i < lightNodes.size(); ++i)
	{
		auto transform = lightNodes[i].transform;
		auto light = lightNodes[i].light;

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
