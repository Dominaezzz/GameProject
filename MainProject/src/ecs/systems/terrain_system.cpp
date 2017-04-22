#include "terrain_system.h"

void TerrainNode::setGameObject(GameObject * gameObject)
{
	Node::setGameObject(gameObject);
	transform = gameObject->getComponent<Transform>();
	terrain = gameObject->getComponent<Terrain>();
}

struct Vertex
{
	Vector3 position;
	Vector2 texCoords;
	Vector3 normal;
};
void generateTerrain(Mesh& mesh)
{
	const size_t VERTEX_COUNT = 128;

	std::array<Vertex, VERTEX_COUNT * VERTEX_COUNT> vertices;
	std::array<int, 6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT * 1)> indexes;

	size_t vertexPointer = 0;
	for (int z = 0; z < VERTEX_COUNT; z++)
	{
		for (int x = 0; x < VERTEX_COUNT; x++)
		{
			Vertex& vertex = vertices[vertexPointer];

			vertex.position = Vector3(-x, 0, -z) / float(VERTEX_COUNT - 1) * float(Terrain::Size);
			vertex.normal = Vector3(0, 1, 0);
			vertex.texCoords = Vector2(x, z) / float(VERTEX_COUNT - 1);

			vertexPointer++;
		}
	}

	size_t pointer = 0;
	for (size_t gz = 0; gz < VERTEX_COUNT - 1; gz++)
	{
		for (size_t gx = 0; gx < VERTEX_COUNT - 1; gx++)
		{
			int topLeft = (gz * VERTEX_COUNT) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = (gz + 1) * VERTEX_COUNT + gx;
			int bottomRight = bottomLeft + 1;

			indexes[pointer++] = topLeft;
			indexes[pointer++] = bottomLeft;
			indexes[pointer++] = topRight;
			indexes[pointer++] = topRight;
			indexes[pointer++] = bottomLeft;
			indexes[pointer++] = bottomRight;
		}
	}

	std::shared_ptr<VertexBuffer> buffer = std::make_shared<VertexBuffer>(vertices.data(), sizeof(vertices));

	mesh.setVertexAttribute<Vector3>(buffer, VertexAttrib::Position, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
	mesh.setVertexAttribute<Vector2>(buffer, VertexAttrib::TexCoords, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texCoords));
	mesh.setVertexAttribute<Vector3>(buffer, VertexAttrib::Normal, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, normal));

	mesh.setIndices(indexes.data(), sizeof(indexes));
}

TerrainSystem::TerrainSystem(World * world) : System(world)
{
	generateTerrain(terrainMesh);

	terrainShader.addVertexShader("res/Shaders/Modular/mainShader.vert", true);
	terrainShader.addVertexShader("res/Shaders/Modular/static.vert", true);
	terrainShader.addVertexShader("res/Shaders/Modular/normal.vert", true);
	
	terrainShader.addFragmentShader("res/Shaders/Modular/mainShader.frag", true);
	terrainShader.addFragmentShader("res/Shaders/Modular/ambient.frag", true);
	terrainShader.addFragmentShader("res/Shaders/Generic/terrainDiffuse.frag", true);
	terrainShader.addFragmentShader("res/Shaders/Modular/specular.frag", true);
	terrainShader.addFragmentShader("res/Shaders/Modular/emissive.frag", true);
	terrainShader.addFragmentShader("res/Shaders/Modular/lighting.frag", true);
	terrainShader.addFragmentShader("res/Shaders/Modular/normal.frag", true);

	terrainShader.compile();

	transormationMatrix = terrainShader.getUniformLocation("transformationMatrix");
	reflectivity = terrainShader.getUniformLocation("reflectivity");
	shininess = terrainShader.getUniformLocation("shininess");
	ambientColor = terrainShader.getUniformLocation("ambientColor");

	terrainShader.begin();
	{
		glUniform1i(terrainShader.getUniformLocation("backgroundTexture"), 0);
		glUniform1i(terrainShader.getUniformLocation("blendMap"), 1);
		glUniform1i(terrainShader.getUniformLocation("rTexture"), 2);
		glUniform1i(terrainShader.getUniformLocation("gTexture"), 3);
		glUniform1i(terrainShader.getUniformLocation("bTexture"), 4);
	}
	terrainShader.end();
}

TerrainSystem::~TerrainSystem()
{
}

void TerrainSystem::update(float dt)
{
}

void TerrainSystem::render()
{
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(true);

	terrainMesh.bind();
	terrainShader.begin();

	glUniform4f(ambientColor, 0.2f, 0.2f, 0.2f, 1.0f);
	glUniform1f(reflectivity, 0.0f);
	glUniform1f(shininess, 0.0f);

	for (size_t i = 0; i < terrainNodes.size(); ++i)
	{
		auto transform = terrainNodes[i].transform;
		auto terrain = terrainNodes[i].terrain;

		const auto trans = transform->getTransform();
		glUniformMatrix4fv(transormationMatrix, 1, false, &trans[0][0]);

		terrain->background->bind(0);
		if (terrain->blendMap != nullptr) terrain->blendMap->bind(1);
		if (terrain->textureR != nullptr) terrain->textureR->bind(2);
		if (terrain->textureG != nullptr) terrain->textureG->bind(3);
		if (terrain->textureB != nullptr) terrain->textureB->bind(4);

		terrainMesh.render(GL_TRIANGLES, 0, -1, false);

		if (terrain->textureB != nullptr) terrain->textureB->unBind(4);
		if (terrain->textureG != nullptr) terrain->textureG->unBind(3);
		if (terrain->textureR != nullptr) terrain->textureR->unBind(2);
		if (terrain->blendMap != nullptr) terrain->blendMap->unBind(1);
		terrain->background->unBind(0);
	}

	terrainShader.end();
	terrainMesh.unBind();
}

void TerrainSystem::onGameObjectEdited(GameObject* gameObject)
{
	terrainNodes.reportChange(gameObject);
}
