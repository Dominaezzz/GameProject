#include "mesh_rendering_system.h"

ShaderProgram * MeshRenderingSystem::getShader(MaterialProperty property)
{
	auto it = shaderPrograms.find(property);
	if(it == shaderPrograms.end())
	{
		auto shaderProgram = std::make_unique<ShaderProgram>();
		std::function<bool(MaterialProperty)> has = [property](MaterialProperty prop) -> bool
		{
			return (property & prop) == prop;
		};

		shaderProgram->attach(modularShaders["mainShader.vert"]);
		shaderProgram->attach(modularShaders["mainShader.frag"]);
		shaderProgram->attach(modularShaders["lighting.frag"]);
		shaderProgram->attach(modularShaders["static.vert"]);
//		shaderProgram->attach(modularShaders[has(MaterialProperty::Skinned) ? "skinning.vert" : "static.vert"]);
		shaderProgram->attach(modularShaders[has(MaterialProperty::TextureAmbient) ? "ambientMap.frag" : "ambient.frag"]);
		shaderProgram->attach(modularShaders[has(MaterialProperty::TextureDiffuse) ? "diffuseMap.frag" : "diffuse.frag"]);
		shaderProgram->attach(modularShaders[has(MaterialProperty::TextureSpecular) ? "specularMap.frag" : "specular.frag"]);
		shaderProgram->attach(modularShaders[has(MaterialProperty::TextureEmissive) ? "emissiveMap.frag" : "emissive.frag"]);
		shaderProgram->attach(modularShaders[has(MaterialProperty::TextureNormal) ? "normalMap.vert" : "normal.vert"]);
		shaderProgram->attach(modularShaders[has(MaterialProperty::TextureNormal) ? "normalMap.frag" : "normal.frag"]);

		shaderProgram->compile();

		shaderProgram->begin();
		{
			if (has(MaterialProperty::TextureAmbient)) glUniform1i(shaderProgram->getUniformLocation("ambientMap"), 0);
			if (has(MaterialProperty::TextureDiffuse)) glUniform1i(shaderProgram->getUniformLocation("diffuseMap"), 1);
			if (has(MaterialProperty::TextureSpecular)) glUniform1i(shaderProgram->getUniformLocation("specularMap"), 2);
			if (has(MaterialProperty::TextureEmissive)) glUniform1i(shaderProgram->getUniformLocation("emissiveMap"), 3);
			if (has(MaterialProperty::TextureNormal)) glUniform1i(shaderProgram->getUniformLocation("normalMap"), 4);
		}
		shaderProgram->end();

		auto sp = shaderProgram.get();
		shaderPrograms[property] = std::move(shaderProgram);
		return sp;
	}
	return it->second.get();
}

MeshRenderingSystem::MeshRenderingSystem(World * world) : System(world)
{
	std::string directory = "res/Shaders/Modular/";

	auto addShader = [=](ShaderType type, const std::string& fileName) -> void
	{
		modularShaders[fileName] = std::make_shared<Shader>(type, directory + fileName);
	};

	addShader(ShaderType::VertexShader, "mainShader.vert");
	addShader(ShaderType::VertexShader, "skinning.vert");
	addShader(ShaderType::VertexShader, "static.vert");
	addShader(ShaderType::VertexShader, "normal.vert");
	addShader(ShaderType::VertexShader, "normalMap.vert");
	addShader(ShaderType::FragmentShader, "mainShader.frag");
	addShader(ShaderType::FragmentShader, "lighting.frag");
	addShader(ShaderType::FragmentShader, "ambient.frag");
	addShader(ShaderType::FragmentShader, "ambientMap.frag");
	addShader(ShaderType::FragmentShader, "diffuse.frag");
	addShader(ShaderType::FragmentShader, "diffuseMap.frag");
	addShader(ShaderType::FragmentShader, "emissive.frag");
	addShader(ShaderType::FragmentShader, "emissiveMap.frag");
	addShader(ShaderType::FragmentShader, "normal.frag");
	addShader(ShaderType::FragmentShader, "normalMap.frag");
	addShader(ShaderType::FragmentShader, "specular.frag");
	addShader(ShaderType::FragmentShader, "specularMap.frag");
}

void MeshRenderingSystem::render()
{
//	std::sort(nodes.begin(), nodes.end(), [](const MeshRenderingNode& left, const MeshRenderingNode& right) -> bool
//	{
//		return left.get<MeshRenderer>()->getProperties() < right.get<MeshRenderer>()->getProperties();
//	});

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(true);

	for (const MeshRenderingNode& node : nodes)
	{
		auto renderer = node.get<MeshRenderer>();

		MaterialProperty properties = renderer->getProperties();

		auto shader = getShader(properties);
		shader->begin();
		{
			glVertexAttrib3f(VertexAttrib::Normal, 0, 1, 0);

			glUniform1f(shader->getUniformLocation("reflectivity"), renderer->getFloat(MaterialProperty::FloatReflectivity));
			glUniform1f(shader->getUniformLocation("shininess"), renderer->getFloat(MaterialProperty::FloatShininess));

			const Color& aColor = renderer->getColor(MaterialProperty::ColorAmbient, Color(0.2f, 0.2f, 0.2f));
			glUniform4fv(shader->getUniformLocation("ambientColor"), 1, reinterpret_cast<const float*>(&aColor));

			const Color& dColor = renderer->getColor(MaterialProperty::ColorDiffuse, Color::White);
			glUniform4fv(shader->getUniformLocation("diffuseColor"), 1, reinterpret_cast<const float*>(&dColor));

			const Color& sColor = renderer->getColor(MaterialProperty::ColorSpecular, Color::Black);
			glUniform4fv(shader->getUniformLocation("specularColor"), 1, reinterpret_cast<const float*>(&sColor));

			const Color& eColor = renderer->getColor(MaterialProperty::ColorEmissive, Color::Transparent);
			glUniform4fv(shader->getUniformLocation("emissiveColor"), 1, reinterpret_cast<const float*>(&eColor));

			auto ambient = renderer->getTexture(MaterialProperty::TextureAmbient);
			auto diffuse = renderer->getTexture(MaterialProperty::TextureDiffuse);
			auto specular = renderer->getTexture(MaterialProperty::TextureSpecular);
			auto emmisive = renderer->getTexture(MaterialProperty::TextureEmissive);
			auto normal = renderer->getTexture(MaterialProperty::TextureNormal);

			if (ambient != nullptr) ambient->bind(0);
			if (diffuse != nullptr) diffuse->bind(1);
			if (specular != nullptr) specular->bind(2);
			if (emmisive != nullptr) emmisive->bind(3);
			if (normal != nullptr) normal->bind(4);

			auto meshFilter = node.get<MeshFilter>();
			meshFilter->mesh->bind();
			{
				GLuint transformMatrix = shader->getUniformLocation("transformationMatrix");
				if (renderer->bones.size() != 0)
				{
					std::vector<Matrix4> bones(renderer->bones.size());
					for (size_t i = 0; i < bones.size(); ++i)
					{
						bones[i] = renderer->bones[i]->getTransform() * meshFilter->bindPoses[i];
					}
					glUniformMatrix4fv(shader->getUniformLocation("bones"), bones.size(), false, &bones[0][0][0]);
					glUniformMatrix4fv(transformMatrix, 1, false, &Matrix4()[0][0]);
				}
				else
				{
					glUniformMatrix4fv(transformMatrix, 1, false, &node.get<Transform>()->getTransform()[0][0]);
				}
				meshFilter->mesh->render(GL_TRIANGLES, 0, -1, false);
			}
			meshFilter->mesh->unBind();

			if (normal != nullptr) normal->unBind(4);
			if (emmisive != nullptr) emmisive->unBind(3);
			if (specular != nullptr) specular->unBind(2);
			if (diffuse != nullptr) diffuse->unBind(1);
			if (ambient != nullptr) ambient->unBind(0);
		}
		shader->end();
	}
}

void MeshRenderingSystem::onGameObjectEdited(GameObject * gameObject)
{
	nodes.reportChange(gameObject);
}
