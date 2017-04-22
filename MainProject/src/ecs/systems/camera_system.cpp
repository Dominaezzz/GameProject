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

	skyBoxMesh.setVertexAttribute<Vector3>(buffer, 0, GL_FLOAT, sizeof(Vector3), 0);
	skyBoxMesh.setIndices(indices, sizeof(indices));

	skyBoxShader.addVertexShader("res/shaders/Generic/skyBox.vert", true);
	skyBoxShader.addFragmentShader("res/shaders/Generic/skyBox.frag", true);
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
	//int count = nodes.Count();
	//if (_matrices.Length < count) Array.Resize(ref _matrices, count);

	for (size_t i = 0; i < cameraNodes.size(); i++)
	{
//		var transform = nodes[i].Transform;
//		_matrices[i] = new Matrices()
//		{
//			Projection = nodes[i].Camera.Projection,
//			View = Matrix4.CreateTranslation(-transform.Position) * Matrix4.CreateFromQuaternion(transform.Rotation.Inverted())
//		};
	}
//	_matrixBuffer.SetData(_matrices);

	glDisable(GL_BLEND);
	glDepthMask(false);
	skyBoxShader.begin();
	for (size_t i = 0; i < cameraNodes.size(); i++)
	{
		//_matrixBuffer.BindRange(0, i * Marshal.SizeOf<Matrices>(), Marshal.SizeOf<Matrices>());

		auto& node = cameraNodes[i];

		Matrices matrices;
		matrices.projection = node.camera->getProjection();
		matrices.view = Matrix4::createFromQuaternion(node.transform->rotation.inverted()) * Matrix4::translate(-node.transform->position);
//		matrices.view = Matrix4::translate(-node.transform->position) * Matrix4::createFromQuaternion(node.transform->rotation.inverted());
		matrices.cameraPosition = node.transform->position;

		cameraBuffer.setSubData(&matrices, 0, sizeof(Matrices));

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
