#ifndef _CAMERA_H_
#define _CAMERA_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Camera.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	Camera
// Description:
//		Camera is preset to a type based on being perspective or orthographic.
//		Used to store the camera's view and projection matrix.
//

//Local Includes
#include "Renderer.h" //<--Includes Dependenies

//Enums:
struct Proj
{
	unsigned int width, height;
	unsigned int x, y;
	float nearDist, farDist, POV;
};

struct View
{
	View() {};

	View(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up)
		: eye(_eye), at(_at), up(_up) {}

	glm::vec3 eye = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 at = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	bool IsEqual(View& _other, float range = 0.005f)
	{
		bool condEye = ((eye.x - _other.eye.x) <= range) && ((eye.y - _other.eye.y) <= range) && ((eye.z - _other.eye.z) <= range);
		bool condAt = ((at.x - _other.at.x) <= range) && ((at.y - _other.at.y) <= range) && ((at.z - _other.at.z) <= range);
		bool condUp = ((up.x - _other.up.x) <= range) && ((up.y - _other.up.y) <= range) && ((up.z - _other.up.z) <= range);

		return condEye && condAt && condUp;
	}
};

class Camera
{
	//Member Functions:
public:
	//Enums
	enum class CamType { Perspective, Orthographic };

	Camera(Camera::CamType type);
	virtual ~Camera();

	virtual void Process(float dt);

	//Sets
	void SetView(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up) { SetView(View(_eye, _at, _up)); };
	void SetView(View _view) { m_view = _view; CalculateViewMatrix(); };
	void SetView(const Camera& _cam) { SetView(_cam.GetView()); };

	View GetView() const { return m_view; };

	void SetProjection(Proj projection);
	void SetPOV(float pov);
	void SetPosition(glm::vec3 position);
	void SetWorldScale(glm::vec3 scale) { m_worldScale = scale; };

	//Gets
	const glm::mat4 GetCamMatrix();
	const glm::mat4 GetViewMatrix() { return m_viewMat; };
	const glm::mat4 GetProjMatrix() { return m_projMat; };
	const glm::vec3 GetWorldPosition();

	//float GetHeight() { return glm::distance(m_targetPos, m_pos); };
	virtual void RemakeMatrix();

	void Shake(float mag, glm::vec3 activeAxis);
	void TransitionView(glm::vec3 position, glm::vec3 forward, glm::vec3 up);
	bool IsInTransition() { return m_isInTransition; };
protected:
	Camera():m_currentProj(), m_type(CamType::Perspective){};

	void SetUpDefault();
	void CalculateViewMatrix();
	void CalculateProjection();
private:
	
	//Member Data:
public:

protected:
	CamType m_type;
	glm::mat4 m_viewMat, m_projMat;

	//Projection
	Proj m_currentProj;

	glm::vec3 m_shakeVector;

	//View
	//glm::vec3 m_pos; //
	glm::vec3 m_targetPos;
	//glm::vec3 m_up;

	View m_view;

	bool m_isInTransition;
	View m_targetView;

	glm::vec3 m_worldScale;

	float timeElapsed = 0.0f;
private:

};


#endif // !_CAMERA_H_
