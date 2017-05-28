#pragma once
#include "../system.h"
#include "../node.h"
#include "../node_list.h"
#include "../../graphics/mesh.h"
#include "../../graphics/buffers/uniform_buffer.h"

class CameraSystem : public System
{
	using CameraNode = Node<Transform, Camera>;
	struct Matrices
	{
		Matrix4 projection;
		Matrix4 view;
		Vector3 cameraPosition;
		const float padding = 0;
	};
	std::vector<Matrices> matriceses = std::vector<Matrices>(1);

	NodeList<CameraNode> cameraNodes = NodeList<CameraNode>(ComponentType<Transform>() | ComponentType<Camera>());
	Mesh skyBoxMesh;
	ShaderProgram skyBoxShader;
	UniformBuffer cameraBuffer = UniformBuffer(sizeof(Matrices), StreamDraw);
public:
	explicit CameraSystem(World* world);
	~CameraSystem() override;
	void render() override;
	void onGameObjectEdited(GameObject* gameObject) override;
};
