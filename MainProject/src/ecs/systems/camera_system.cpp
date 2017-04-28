#include "camera_system.h"

void CameraNode::setGameObject(GameObject * gameObject)
{
	Node::setGameObject(gameObject);
	transform = gameObject->getComponent<Transform>();
	camera = gameObject->getComponent<Camera>();
}

CameraSystem::CameraSystem(World* world) : System(world)
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
	
	auto buffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));

	skyBoxMesh.setVertexAttribute<Vector3>(buffer, VertexAttrib::Position, sizeof(Vector3), 0);
	skyBoxMesh.setIndices(indices, sizeof(indices));

	skyBoxShader.add(ShaderType::VertexShader, "res/shaders/Generic/skyBox.vert");
	skyBoxShader.add(ShaderType::FragmentShader, "res/shaders/Generic/skyBox.frag");
	skyBoxShader.compile();

	cameraBuffer.bindBase(0);
}

CameraSystem::~CameraSystem()
{
}

void CameraSystem::update(float dt)
{
}

void CameraSystem::render()
{
	std::vector<Matrices> matriceses(cameraNodes.size());

	for (size_t i = 0; i < cameraNodes.size(); i++)
	{
		auto& node = cameraNodes[i];
		matriceses[i].projection = node.camera->getProjection();
		matriceses[i].view = Matrix4::createFromQuaternion(node.transform->Rotation.inverted()) * Matrix4::translate(-node.transform->Position);
		matriceses[i].cameraPosition = node.transform->getWorldPosition();
	}
	cameraBuffer.setSubData(matriceses.data(), 0, sizeof(Matrices) * cameraNodes.size());

	glDisable(GL_BLEND);
	glDepthMask(false);
	skyBoxShader.begin();
	for (size_t i = 0; i < cameraNodes.size(); i++)
	{
		cameraBuffer.bindRange(0, i * sizeof(Matrices), sizeof(Matrices));

		auto& node = cameraNodes[i];

		node.camera->skyBox->bind(0);
		skyBoxMesh.render(GL_TRIANGLES, 0, 36);
		node.camera->skyBox->unBind(0);
	}
	skyBoxShader.end();
	glDepthMask(true);
	glEnable(GL_BLEND);
}

void CameraSystem::onGameObjectEdited(GameObject * gameObject)
{
	cameraNodes.reportChange(gameObject);
}
