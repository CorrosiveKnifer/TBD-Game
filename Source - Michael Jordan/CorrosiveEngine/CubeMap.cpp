#include "CubeMap.h"

#include "Camera.h"
#include "Texture.h"
#include "MeshEntity.h"
#include <stb_image.h>

CubeMap::CubeMap(Camera& _cam, Texture& _texture)
{
	m_MainCamera = &_cam;

	m_program = &Renderer::GetInstance().CreateProgram("CubeMap.vs", "CubeMap.fs");

	MeshEntity* mE = new MeshEntity(Renderer::GetInstance().CreateCubeMapMesh());
	mE->SetTexture("cubeSampler", _texture);
	m_subEntities.push_back(mE);

	m_texture = &_texture;
}

CubeMap::~CubeMap()
{
	//Camera not owned
	m_MainCamera = 0;
}

void CubeMap::Draw(Camera* camera, Program* program)
{
	//Ignore any program incoming, use my own!
	m_program->Use();

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	//m_program->SetMatrix4("transform", camera->GetCamMatrix() * m_transform);

	Entity::Draw(camera, m_program);
	//m_program->BindVertextArray(m_VAO);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//m_program->Clear();
}

void CubeMap::Process(float dt)
{
	Entity::Process(dt);
}
