#ifndef _CUBE_MAP_H_
#define _CUBE_MAP_H_

#include "Renderer.h"
#include "Entity.h"
#include "Program.h"
#include <string>

class Camera;

class CubeMap : public Entity
{
	//Member Functions:
public:
	CubeMap(Camera& _cam, Texture& _texture);
	~CubeMap();

	virtual void Draw(Camera* camera, Program* program);
	virtual void Process(float dt);

	Texture& GetTexture() { return *m_texture; };
protected:

private:

	//Member Data:
public:

protected:
	Camera* m_MainCamera;

	Program* m_program;
	//GLuint m_VAO;
	Texture* m_texture;

	glm::mat4 m_transform;

private:

};

#endif // !_CUBE_MAP_H_

