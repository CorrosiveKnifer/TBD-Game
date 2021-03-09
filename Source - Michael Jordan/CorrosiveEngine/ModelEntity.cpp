#include "ModelEntity.h"
#include "Texture.h"

ModelEntity::ModelEntity()
{
	m_model = 0;

}

ModelEntity::ModelEntity(Model& modelRef)
{
	m_model = &modelRef;
	//m_model->SetTexture(Renderer::GetInstance().CreateTexture("DefaultTexture.png"));
}

ModelEntity::~ModelEntity()
{
	//model is owned by renderer
}

void ModelEntity::Draw(Camera* camera, Program* program)
{
	Transform _worldTransform = Transform::ExtractTransform(m_inheritMatrix * m_localTransform.GetModelMatrix());
	
	m_model->Draw(camera, program, _worldTransform);

	Entity::Draw(camera, program);
}

void ModelEntity::Process(float dt)
{
	Entity::Process(dt);
}

void ModelEntity::SetColour(glm::vec4 _colour)
{
	m_model->SetColour(_colour);
}
