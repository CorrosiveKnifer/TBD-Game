#ifndef _MODEL_ENTITY_H_
#define _MODEL_ENTITY_H_

#include "Entity.h"
#include "Model.h"


class ModelEntity : public Entity
{
public:
	ModelEntity();
	ModelEntity(Model& modelRef);
	~ModelEntity();

	virtual void Draw(Camera* cam, Program* program);
	virtual void Process(float dt);

	void SetColour(glm::vec4 _colour);
protected:

private:
	//Member Data
public:

protected:
	Model* m_model;
private:

};

#endif // !_MODEL_ENTITY_H_