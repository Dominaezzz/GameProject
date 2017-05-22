#pragma once
#include "../components/transform.h"
#include "../components/light.h"
#include "../node.h"
#include "../../graphics/buffers/uniform_buffer.h"
#include "../node_list.h"

class LightSystem : public System
{
	using LightNode = Node<Transform, Light>;

	struct Lights
	{
		struct BaseLight
		{
			Vector3 color;
			float intensity = 0;
		};
		static_assert(sizeof(BaseLight) == 16, "Size is incorrect");

		struct DirectionalLight
		{
			BaseLight baseLight;
			Vector3 direction;
			const float padding = 0;
		} directionalLights[2];
		static_assert(sizeof(DirectionalLight) == 32, "Size is incorrect");

		struct PointLight
		{
			BaseLight baseLight;
			Attenuation attenuation;
			const float padding = 0;
			Vector3 position;
			float radius = 0;
		} pointLights[4];
		static_assert(sizeof(PointLight) == 48, "Size is incorrect");

		struct SpotLight
		{
			PointLight pointLight;
			Vector3 direction;
			float cutOff = 0;
		} spotLights[4];
		static_assert(sizeof(SpotLight) == 64, "Size is incorrect");
	} lights;
	static_assert(sizeof(Lights) == 512, "Size is incorrect");

	UniformBuffer lightsBuffer = UniformBuffer(sizeof(Lights), StreamDraw);
	NodeList<LightNode> lightNodes = NodeList<LightNode>(ComponentType<Transform>() | ComponentType<Light>());

	static void setBaseLight(Lights::BaseLight& baseLight, const Light& light);
	static void setDirectionalLight(Lights::DirectionalLight& directionalLight, const Transform& transform, const Light& light);
	static void setPointLight(Lights::PointLight& pointLight, const Transform& transform, const Light& light);
	static void setSpotLight(Lights::SpotLight& spotLight, const Transform& transform, const Light& light);
public:
	explicit LightSystem(World* world);
	~LightSystem() override;
	void update(float dt) override;
	void render() override;
	void onGameObjectEdited(GameObject* gameObject) override;
};
