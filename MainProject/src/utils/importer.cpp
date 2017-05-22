#include "importer.h"

#define TINYGLTF_LOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "yocto_gltf.h"

using namespace ygltf;

std::shared_ptr<Mesh> loadMesh(glTF_t* gltf, const mesh_t& mesh, std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers)
{
	auto modelMesh = std::make_shared<Mesh>();

	const auto& primitive = mesh.primitives[0];

	auto it = primitive.attributes.find("POSITION");
	if (it != primitive.attributes.end())
	{
		const accessor_t& accessor = gltf->accessors[it->second];

		int stride = gltf->bufferViews[accessor.bufferView].byteStride;
		modelMesh->setVertexAttribute<Vector3>(vertexBuffers[accessor.bufferView], VertexAttrib::Position, stride, accessor.byteOffset);
	}

	it = primitive.attributes.find("TEXCOORD_0");
	if (it != primitive.attributes.end())
	{
		const accessor_t& accessor = gltf->accessors[it->second];

		int stride = gltf->bufferViews[accessor.bufferView].byteStride;
		modelMesh->setVertexAttribute<Vector2>(vertexBuffers[accessor.bufferView], VertexAttrib::TexCoords, stride, accessor.byteOffset);
	}

	it = primitive.attributes.find("NORMAL");
	if (it != primitive.attributes.end())
	{
		const accessor_t& accessor = gltf->accessors[it->second];

		int stride = gltf->bufferViews[accessor.bufferView].byteStride;
		modelMesh->setVertexAttribute<Vector3>(vertexBuffers[accessor.bufferView], VertexAttrib::Normal, stride, accessor.byteOffset);
	}

	it = primitive.attributes.find("TANGENT");
	if (it != primitive.attributes.end())
	{
		const accessor_t& accessor = gltf->accessors[it->second];

		int stride = gltf->bufferViews[accessor.bufferView].byteStride;
		modelMesh->setVertexAttribute<Vector3>(vertexBuffers[accessor.bufferView], VertexAttrib::Tangent, stride, accessor.byteOffset);
	}

	it = primitive.attributes.find("BITANGENT");
	if (it != primitive.attributes.end())
	{
		const accessor_t& accessor = gltf->accessors[it->second];

		int stride = gltf->bufferViews[accessor.bufferView].byteStride;
		modelMesh->setVertexAttribute<Vector3>(vertexBuffers[accessor.bufferView], VertexAttrib::BiTangent, stride, accessor.byteOffset);
	}

	it = primitive.attributes.find("WEIGHT");
	if (it != primitive.attributes.end())
	{
		const accessor_t& accessor = gltf->accessors[it->second];

		int stride = gltf->bufferViews[accessor.bufferView].byteStride;
		modelMesh->setVertexAttribute<Vector4>(vertexBuffers[accessor.bufferView], VertexAttrib::Weights, stride, accessor.byteOffset);
	}

	it = primitive.attributes.find("JOINT");
	if (it != primitive.attributes.end())
	{
		const accessor_t& accessor = gltf->accessors[it->second];

		int stride = gltf->bufferViews[accessor.bufferView].byteStride;
		modelMesh->setVertexAttribute<Vector4>(vertexBuffers[accessor.bufferView], VertexAttrib::Joints, stride, accessor.byteOffset);
	}


	if (primitive.indices != -1)
	{
		const accessor_t& accessor = gltf->accessors[primitive.indices];
		const bufferView_t& bufferView = gltf->bufferViews[accessor.bufferView];
		const buffer_t& buffer = gltf->buffers[bufferView.buffer];

		std::vector<int> indexes(accessor.count);

		if (accessor.componentType == accessor_t::componentType_t::unsigned_short_t)
		{
			auto temp = reinterpret_cast<const short*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
			for (int i = 0; i < accessor.count; ++i)
			{
				indexes[i] = temp[i];
			}
		}
		else if (accessor.componentType == accessor_t::componentType_t::unsigned_int_t)
		{
			auto temp = reinterpret_cast<const unsigned int*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
			for (int i = 0; i < accessor.count; ++i)
			{
				indexes[i] = temp[i];
			}
		}

		modelMesh->setIndices(indexes.data(), accessor.count * sizeof(int));
	}

	return modelMesh;
}

std::shared_ptr<Texture2D> loadTexture(glTF_t* gltf, const texture_t& texture)
{
	const image_t& image = gltf->images[texture.source];
	const sampler_t& sampler = gltf->samplers[texture.sampler];

	PixelFormat format;
	switch (image.data.ncomp)
	{
	case 1:
		format = LUMINANCE;
		break;
	case 2:
		format = LUMINANCE_ALPHA;
		break;
	case 3:
		format = RGB;
		break;
	case 4:
		format = RGBA;
		break;
	default:
		throw std::runtime_error("None matching components: " + std::to_string(image.data.ncomp));
	}

	auto modelTexture = std::make_shared<Texture2D>(image.data.width, image.data.height, static_cast<PixelFormat>(texture.internalFormat));

	modelTexture->setData<unsigned char>(image.data.datab.data(), static_cast<PixelFormat>(format));
	modelTexture->setFilter(static_cast<MinFilter>(sampler.minFilter), static_cast<MagFilter>(sampler.magFilter));
	modelTexture->setWrap(static_cast<Wrap>(sampler.wrapS), static_cast<Wrap>(sampler.wrapT));

	switch (sampler.minFilter)
	{
	case sampler_t::minFilter_t::nearest_mipmap_nearest_t:
	case sampler_t::minFilter_t::linear_mipmap_nearest_t:
	case sampler_t::minFilter_t::nearest_mipmap_linear_t:
	case sampler_t::minFilter_t::linear_mipmap_linear_t:
		modelTexture->generateMipmap();
		break;
	case sampler_t::minFilter_t::nearest_t: break;
	case sampler_t::minFilter_t::linear_t: break;
	default:;
	}

	return modelTexture;
}

std::shared_ptr<Material> loadMaterial(const material_t& material, std::vector<std::shared_ptr<Texture2D>> textures)
{
	std::shared_ptr<Material> modelMaterial = std::make_shared<Material>();
	auto it = material.extensions.find("KHR_materials_common");
	if (it == material.extensions.end()) return std::move(modelMaterial);

	const nlohmann::json& mat_common = it->second;
	const nlohmann::json& values = mat_common["values"];

	if (values.count("transparency")) modelMaterial->setFloat(MaterialProperty::FloatReflectivity, values["transparency"].at(0));
	if (values.count("shininess")) modelMaterial->setFloat(MaterialProperty::FloatShininess, values["shininess"].at(0));
	
	if (values.count("diffuse"))
	{
		const nlohmann::json& diffuse = values["diffuse"];
		if (diffuse.is_number_integer())
		{
			modelMaterial->setTexture(MaterialProperty::TextureDiffuse, textures[diffuse]);
		}
		else if (diffuse.is_array())
		{
			if (diffuse.size() == 1)
			{
				modelMaterial->setTexture(MaterialProperty::TextureDiffuse, textures[diffuse.at(0)]);
			}
			else if (diffuse.size() == 4)
			{
				Color color = Color(diffuse.at(0), diffuse.at(1), diffuse.at(2), diffuse.at(3));
				modelMaterial->setColor(MaterialProperty::ColorDiffuse, color);
			}
		}
	}
	if (values.count("specular"))
	{
		const nlohmann::json& specular = values["specular"];
		if (specular.is_number_integer())
		{
			modelMaterial->setTexture(MaterialProperty::TextureSpecular, textures[specular]);
		}
		else if (specular.is_array())
		{
			if (specular.size() == 1)
			{
				modelMaterial->setTexture(MaterialProperty::TextureSpecular, textures[specular.at(0)]);
			}
			else if (specular.size() == 4)
			{
				Color color(specular.at(0), specular.at(1), specular.at(2), specular.at(3));
				modelMaterial->setColor(MaterialProperty::ColorSpecular, color);
			}
		}
	}
	
	return std::move(modelMaterial);
}

GameObject * Importer::load(World & world, const std::string& path)
{
	glTF_t* gltf = ygltf::load_gltf(path);

	std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
	std::vector<std::shared_ptr<Texture2D>> textures;
	std::vector<std::shared_ptr<Material>> materials;
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<GameObject*> nodes;

	for (const bufferView_t& bufferView : gltf->bufferViews)
	{
		const buffer_t& buffer = gltf->buffers[bufferView.buffer];
		if (bufferView.target == bufferView_t::target_t::array_buffer_t)
		{
			vertexBuffers.push_back(std::make_shared<VertexBuffer>(
				&buffer.data[bufferView.byteOffset],
				bufferView.byteLength
			));
		}
		else vertexBuffers.push_back(nullptr);
	}
	for (const texture_t& texture : gltf->textures) textures.push_back(loadTexture(gltf, texture));
	for (const material_t& material : gltf->materials) materials.push_back(loadMaterial(material, textures));
	for (const mesh_t& mesh : gltf->meshes) meshes.push_back(loadMesh(gltf, mesh, vertexBuffers));

	// Parse in nodes.
	for (const node_t& node : gltf->nodes)
	{
		GameObject* object = world.newGameObject();
		{
			Transform* transform = object->addComponent<Transform>();
			if(node.skin != -1)
			{
				transform->Position = reinterpret_cast<const Vector3&>(node.translation);
				transform->Scale = reinterpret_cast<const Vector3&>(node.scale);
				transform->Rotation = reinterpret_cast<const Quaternion&>(node.rotation);
			}
			else
			{
				const Matrix4& matrix = reinterpret_cast<const Matrix4&>(node.matrix);
				transform->Position = matrix.extractTranslation();
				transform->Rotation = matrix.extractRotation();
				transform->Scale = matrix.extractScale();
			}

			if (node.mesh != -1)
			{
				MeshFilter* filter = object->addComponent<MeshFilter>();
				filter->mesh = meshes[node.mesh];

				MeshRenderer* renderer = object->addComponent<MeshRenderer>();
				renderer->material = materials[gltf->meshes[node.mesh].primitives[0].material];

				if (node.skin != -1)
				{
					const skin_t& skin = gltf->skins[node.skin];
					const accessor_t& accessor = gltf->accessors[skin.inverseBindMatrices];
					const bufferView_t& bufferView = gltf->bufferViews[accessor.bufferView];
					const buffer_t& buffer = gltf->buffers[bufferView.buffer];

					for (int i = 0; i < accessor.count; ++i)
					{
						filter->bindPoses.push_back(*reinterpret_cast<const Matrix4*>(
							&buffer.data[bufferView.byteOffset + (i * bufferView.byteStride) + accessor.byteOffset]
						));
					}

					for (int joint : skin.joints)
					{
						renderer->bones.push_back(nodes[joint]->getComponent<Transform>());
					}
				}
			}
		}
		nodes.push_back(object);
	}
	// Assign parents.
	for (int i = 0; i < gltf->nodes.size(); ++i)
	{
		Transform* parent = nodes[i]->getComponent<Transform>();
		for (int childIndex : gltf->nodes[i].children)
		{
			parent->addChild(nodes[childIndex]->getComponent<Transform>());
		}
	}

	GameObject* root = nullptr;

	const scene_t& mainScene = gltf->scenes[gltf->scene];
	if (mainScene.nodes.size() == 1)
	{
		root = nodes[mainScene.nodes[0]];
	}
	else
	{
		root = world.newGameObject();
		Transform* transform = root->addComponent<Transform>();
		for (int rootNode : mainScene.nodes) transform->addChild(nodes[rootNode]->getComponent<Transform>());
	}

	for (const animation_t& animation : gltf->animations)
	{
		Animator* animator = root->addComponent<Animator>();
		for (const animation_channel_t& channel : animation.channels)
		{
			const animation_channel_target_t& target = channel.target;
			const animation_sampler_t& sampler = animation.samplers[channel.sampler];

			// Time values
			const accessor_t& in_accessor = gltf->accessors[sampler.input];
			const bufferView_t& in_buffer_view = gltf->bufferViews[in_accessor.bufferView];
			const buffer_t& in_buffer = gltf->buffers[in_buffer_view.buffer];

			// Key Frame values
			const accessor_t& out_accessor = gltf->accessors[sampler.output];
			const bufferView_t& out_buffer_view = gltf->bufferViews[out_accessor.bufferView];
			const buffer_t& out_buffer = gltf->buffers[out_buffer_view.buffer];

			animator->duration = std::max(animator->duration, in_accessor.max[0]);
			NodeAnimation nodeAnimation;
			nodeAnimation.transform = nodes[target.node]->getComponent<Transform>();

			int in_offset = in_buffer_view.byteOffset + in_accessor.byteOffset;
			int in_stride = in_buffer_view.byteStride;
			if (in_stride == 0) in_stride = sizeof(float);

			int out_offset = out_buffer_view.byteOffset + out_accessor.byteOffset;

			if (target.path == animation_channel_target_t::path_t::translation_t)
			{
				int out_stride = out_buffer_view.byteStride == 0 ? sizeof(Vector3) : out_buffer_view.byteStride;

				for (int i = 0; i < in_accessor.count; ++i)
				{
					float time = *reinterpret_cast<const float*>(&in_buffer.data[in_offset + (i * in_stride)]);
					const Vector3& value = *reinterpret_cast<const Vector3*>(&out_buffer.data[out_offset + (i * out_stride)]);

					nodeAnimation.translations.insert(KeyFrame<Vector3>(time, value));
				}
			}
			else if (target.path == animation_channel_target_t::path_t::scale_t)
			{
				int out_stride = out_buffer_view.byteStride == 0 ? sizeof(Vector3) : out_buffer_view.byteStride;

				for (int i = 0; i < in_accessor.count; ++i)
				{
					float time = *reinterpret_cast<const float*>(&in_buffer.data[in_offset + (i * in_stride)]);
					const Vector3& value = *reinterpret_cast<const Vector3*>(&out_buffer.data[out_offset + (i * out_stride)]);

					nodeAnimation.scales.insert(KeyFrame<Vector3>(time, value));
				}
			}
			else if (target.path == animation_channel_target_t::path_t::rotation_t)
			{
				int out_stride = out_buffer_view.byteStride == 0 ? sizeof(Quaternion) : out_buffer_view.byteStride;

				for (int i = 0; i < in_accessor.count; ++i)
				{
					float time = *reinterpret_cast<const float*>(&in_buffer.data[in_offset + (i * in_stride)]);
					const Quaternion& value = *reinterpret_cast<const Quaternion*>(&out_buffer.data[out_offset + (i * out_stride)]);

					nodeAnimation.rotations.insert(KeyFrame<Quaternion>(time, value));
				}
			} else continue;

			animator->nodeAnimations.push_back(std::move(nodeAnimation));
		}
		break;
	}
	
	return root;
}
