#pragma once
#include "../node.h"
#include "../components/terrain.h"
#include "../node_list.h"
#include "../../graphics/shaderprogram.h"
#include "../../graphics/mesh.h"

struct TerrainNode final : Node
{
	Transform* transform;
	Terrain* terrain;

	void setGameObject(GameObject* gameObject) override;
};

class TerrainSystem : public System
{
	NodeList<TerrainNode> terrainNodes = NodeList<TerrainNode>(ComponentType<Transform>() | ComponentType<Terrain>());
	ShaderProgram terrainShader;
	Mesh terrainMesh;

	GLuint transormationMatrix;
	GLuint reflectivity;
	GLuint shininess;
	GLuint ambientColor;

public:
	explicit TerrainSystem(World* world);
	~TerrainSystem() override;
	void update(float dt) override;
	void render() override;
	void onGameObjectEdited(GameObject* gameObject) override;
};
