#include "terrain_system.h"

void generateTerrain(Mesh& mesh)
{
	struct Vertex
	{
		Vector3 position;
		Vector2 texCoords;
		Vector3 normal;
	};

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

	mesh.setVertexBuffer(0, std::make_shared<VertexBuffer>(vertices.data(), sizeof(vertices)), sizeof(Vertex));

	mesh.setAttribute<Vector3>(0, VertexAttrib::Position, offsetof(Vertex, position));
	mesh.setAttribute<Vector2>(0, VertexAttrib::TexCoords, offsetof(Vertex, texCoords));
	mesh.setAttribute<Vector3>(0, VertexAttrib::Normal, offsetof(Vertex, normal));

	mesh.setIndices(indexes.data(), sizeof(indexes));

	mesh.count = indexes.size();
}

TerrainSystem::TerrainSystem(World * world) : System(world)
{
	generateTerrain(terrainMesh);

	terrainShader.add(ShaderType::VertexShader, "res/Shaders/Modular/mainShader.vert");
	terrainShader.add(ShaderType::VertexShader, "res/Shaders/Modular/static.vert");
	terrainShader.add(ShaderType::VertexShader, "res/Shaders/Modular/normal.vert");
	
	terrainShader.add(ShaderType::FragmentShader, "res/Shaders/Modular/mainShader.frag");
	terrainShader.add(ShaderType::FragmentShader, "res/Shaders/Modular/ambient.frag");
	terrainShader.add(ShaderType::FragmentShader, "res/Shaders/Generic/terrainDiffuse.frag");
	terrainShader.add(ShaderType::FragmentShader, "res/Shaders/Modular/specular.frag");
	terrainShader.add(ShaderType::FragmentShader, "res/Shaders/Modular/emissive.frag");
	terrainShader.add(ShaderType::FragmentShader, "res/Shaders/Modular/lighting.frag");
	terrainShader.add(ShaderType::FragmentShader, "res/Shaders/Modular/normal.frag");

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
		auto transform = terrainNodes[i].get<Transform>();
		auto terrain = terrainNodes[i].get<Terrain>();

		const auto trans = transform->getTransform();
		glUniformMatrix4fv(transormationMatrix, 1, false, &trans[0][0]);

		terrain->background->bind(0);
		if (terrain->blendMap != nullptr) terrain->blendMap->bind(1);
		if (terrain->textureR != nullptr) terrain->textureR->bind(2);
		if (terrain->textureG != nullptr) terrain->textureG->bind(3);
		if (terrain->textureB != nullptr) terrain->textureB->bind(4);

		terrainMesh.render(false);

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
