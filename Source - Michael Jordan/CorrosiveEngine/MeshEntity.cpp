#include "MeshEntity.h"

MeshEntity::MeshEntity()
{
	m_mesh = 0;
	m_colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

MeshEntity::MeshEntity(Mesh& meshRef)
{
	m_mesh = &meshRef;
	m_colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

MeshEntity::~MeshEntity()
{
	//Mesh owned by renderer
	m_mesh = nullptr;

	//Textures are not owned by this mesh, deleted in Renderer
	m_textureRef.clear();
}

void MeshEntity::Draw(Camera* cam, Program* program)
{
	Transform _worldTransform = Transform::ExtractTransform(m_inheritMatrix * m_localTransform.GetModelMatrix());

	program->Use();

	m_mesh->SetColour(m_colour);

	//Apply textures
	int i = 0;
	for (auto iter : m_textureRef)
	{
		iter.second->ApplyTexture(iter.first, GL_TEXTURE0+i, program);
	
		if (i >= 32) //Opengl Limit
			break;
	
		i++; //Iterate to the next slot
	}

	m_mesh->Draw(cam, program, _worldTransform);

	program->Clear();

	Entity::Draw(cam, program);
}

void MeshEntity::Process(float dt)
{
	Entity::Process(dt);
}

void MeshEntity::SetTexture(std::string name, Texture& texture)
{
	if (m_textureRef.find(name) == m_textureRef.end())
	{
		m_textureRef.insert(std::pair<std::string, Texture*>(name, &texture));
		return;
	}
	m_textureRef.at(name) = &texture;
}
