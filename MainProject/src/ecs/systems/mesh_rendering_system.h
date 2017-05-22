#pragma once
#include "../system.h"
#include "../node.h"
#include "../node_list.h"

class MeshRenderingSystem : public System
{
	using MeshRenderingNode = Node<Transform, MeshFilter, MeshRenderer>;

	NodeList<MeshRenderingNode> nodes = NodeList<MeshRenderingNode>(
		ComponentType<Transform>() | ComponentType<MeshFilter>() | ComponentType<MeshRenderer>()
	);

	std::unordered_map<MaterialProperty, std::unique_ptr<ShaderProgram>> shaderPrograms;
	std::unordered_map<std::string, std::shared_ptr<Shader>> modularShaders;

	ShaderProgram* getShader(MaterialProperty property);
public:
	explicit MeshRenderingSystem(World* world);
	~MeshRenderingSystem() override = default;
	void render() override;
	void onGameObjectEdited(GameObject* gameObject) override;
};
