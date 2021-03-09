#ifndef _MESH_ENTITY_H_
#define _MESH_ENTITY_H_

#include "Entity.h"
#include "Mesh.h"
#include "Texture.h"

class MeshEntity : public Entity
{
public:
	MeshEntity();
	MeshEntity(Mesh& meshRef);
	~MeshEntity();

	virtual void Draw(Camera* cam, Program* program);
	virtual void Process(float dt);

	void SetColour(glm::vec4 _newColour) { m_colour = _newColour; };

	void SetTexture(std::string name, Texture& texture);
	void SetTexture(Texture& texture) { SetTexture("imageTexture", texture); };
	const Texture* GetTexture(std::string name = "imageTexture") { return m_textureRef.at(name); };
protected:

private:

	//Member Data
public:

protected:
	Mesh* m_mesh;

	std::map<std::string, Texture*> m_textureRef;

	glm::vec4 m_colour;
private:

};

#endif // !_MESH_ENTITY_H_