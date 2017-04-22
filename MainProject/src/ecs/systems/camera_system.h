#pragma once
#include "../system.h"
#include "../node.h"
#include "../node_list.h"
#include "../../graphics/mesh.h"
#include "../../graphics/buffers/uniform_buffer.h"

struct CameraNode final : Node
{
	Transform* transform;
	Camera* camera;

	void setGameObject(GameObject* gameObject) override;
};

class CameraSystem : public System
{
	struct Matrices
	{
		Matrix4 projection;
		Matrix4 view;
		Vector3 cameraPosition;
		const float padding = 0;
	};

	NodeList<CameraNode> cameraNodes = NodeList<CameraNode>(ComponentType<Transform>() | ComponentType<Camera>());
	Mesh skyBoxMesh;
	ShaderProgram skyBoxShader;
	UniformBuffer cameraBuffer = UniformBuffer(sizeof(Matrices), StreamDraw);
public:
	explicit CameraSystem(World* world);
	~CameraSystem() override;
	void update(float dt) override;
	void render() override;
	void onGameObjectEdited(GameObject* gameObject) override;
};
