#include "BoxCollider.h"
#include "Camera.h"

BoxCollider::BoxCollider()
{
	//m_VAO = Renderer::GetInstance().CreateCubeMapMesh();
}

BoxCollider::~BoxCollider()
{

}

void BoxCollider::Draw(Camera* _cam)
{
	m_program->Use();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::mat4 MVP = (_cam != 0) ? _cam->GetCamMatrix() : glm::mat4();

	m_program->SetVector4("colorInput", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	m_program->SetMatrix4("transform", MVP * m_transform.GetModelMatrix());
	
	//m_program->BindVertextArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_program->Clear();
}
