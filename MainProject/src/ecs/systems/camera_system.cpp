#include "camera_system.h"

CameraSystem::CameraSystem(World* world) : System(world), cameraBuffer(sizeof(Matrices), StreamDraw)
{
	const int indices[] =
	{
		0, 1, 2,  2, 3, 0,
		4, 1, 0,  0, 5, 4,
		2, 6, 7,  7, 3, 2,
		4, 5, 7,  7, 6, 4,
		0, 3, 7,  7, 5, 0,
		1, 4, 2,  2, 4, 6
	};

	const float Size = 500;
	const Vector3 vertices[] = {
		Vector3(-Size,  Size, -Size), Vector3(-Size, -Size, -Size),
		Vector3( Size, -Size, -Size), Vector3( Size,  Size, -Size),
		
		Vector3(-Size, -Size,  Size), Vector3(-Size,  Size,  Size),
		Vector3( Size, -Size,  Size), Vector3( Size,  Size,  Size)
	};

	const auto buffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));

	skyBoxMesh.setVertexAttribute<Vector3>(buffer, VertexAttrib::Position, sizeof(Vector3), 0);
	skyBoxMesh.setIndices(indices, sizeof(indices));
	skyBoxMesh.count = 36;

	skyBoxShader.add(ShaderType::VertexShader, "res/shaders/Generic/skyBox.vert");
	skyBoxShader.add(ShaderType::FragmentShader, "res/shaders/Generic/skyBox.frag");
	skyBoxShader.compile();

	cameraBuffer.bindBase(0);
}

CameraSystem::~CameraSystem()
{
}

void CameraSystem::render()
{
	if (cameraNodes.size() > matriceses.size())
	{
		matriceses.resize(cameraNodes.size());
		cameraBuffer.setData(nullptr, sizeof(Matrices) * cameraNodes.size(), BufferUsage::StreamDraw);
	}

	for (size_t i = 0; i < cameraNodes.size(); i++)
	{
		auto& node = cameraNodes[i];
		matriceses[i].projection = node.get<Camera>()->getProjection();
		auto transform = node.get<Transform>();
		matriceses[i].view = Matrix4::createFromQuaternion(transform->Rotation.inverted()) * Matrix4::translate(-transform->Position);
		matriceses[i].cameraPosition = transform->getWorldPosition();
	}
	cameraBuffer.setSubData(matriceses.data(), 0, sizeof(Matrices) * cameraNodes.size());

	glDisable(GL_BLEND);
	glDepthMask(false);
	skyBoxShader.begin();
	for (size_t i = 0; i < cameraNodes.size(); i++)
	{
		cameraBuffer.bindRange(0, i * sizeof(Matrices), sizeof(Matrices));

		auto& node = cameraNodes[i];

		node.get<Camera>()->skyBox->bind(0);
		skyBoxMesh.render();
		node.get<Camera>()->skyBox->unBind(0);
	}
	skyBoxShader.end();
	glDepthMask(true);
	glEnable(GL_BLEND);
}

void CameraSystem::onGameObjectEdited(GameObject * gameObject)
{
	cameraNodes.reportChange(gameObject);
}
