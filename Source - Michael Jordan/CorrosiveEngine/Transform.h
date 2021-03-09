#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Renderer.h"

class Transform
{
public:
	Transform(glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::quat _rot = glm::quat(0.0f, 0.0f, 0.0f, 0.0f), glm::vec3 _sca = glm::vec3(1.0f, 1.0f, 1.0f))
	{
		m_position = _pos;
		m_rotation = _rot;
		m_scale = _sca;
	}

	~Transform()
	{

	}

	void SetPosition(glm::vec3 _pos)
	{
		m_position = _pos;
	}
	void SetRotation(glm::quat _rot)
	{
		m_rotation = _rot;
	}
	void SetScale(glm::vec3 _scale)
	{
		m_scale = _scale;
	}

	static Transform ExtractTransform(glm::mat4 _matrix)
	{
		glm::vec3 position = glm::vec3(_matrix[3].x, _matrix[3].y, _matrix[3].z);

		glm::vec3 scaleX = glm::vec3(_matrix[0].x, _matrix[0].y, _matrix[0].z);
		glm::vec3 scaleY = glm::vec3(_matrix[1].x, _matrix[1].y, _matrix[1].z);
		glm::vec3 scaleZ = glm::vec3(_matrix[2].x, _matrix[2].y, _matrix[2].z);

		glm::vec3 scale = glm::vec3(glm::length(scaleX), glm::length(scaleY), glm::length(scaleZ));

		_matrix[0] = _matrix[0] / scale.x;
		_matrix[1] = _matrix[1] / scale.y;
		_matrix[2] = _matrix[2] / scale.z;
		_matrix[3] = glm::vec4();

		glm::quat rotation = glm::toQuat(_matrix);

		return Transform(position, rotation, scale);
	}

	glm::mat4 GetModelMatrix()
	{
		//Translate * Rotation * Scale
		return glm::translate(glm::mat4(), m_position) * glm::toMat4(m_rotation) * glm::mat4(glm::scale(glm::mat4(), m_scale));
	};

	glm::vec3 GetForward() 
	{
		glm::mat4 matrix = glm::translate(glm::mat4(), m_position) * glm::toMat4(m_rotation);
		return glm::normalize(glm::vec3(matrix[2].x, matrix[2].y, matrix[2].z));
	};
	
	glm::vec3 GetRight()
	{
		glm::mat4 matrix = GetModelMatrix();
		return glm::normalize(glm::vec3(matrix[1].x, matrix[1].y, matrix[1].z));
	};

	glm::vec3 GetUp()
	{
		glm::mat4 matrix = GetModelMatrix();
		return glm::normalize(glm::vec3(matrix[0].x, matrix[0].y, matrix[0].z));
	};

	glm::vec3& GetPositionRef() { return m_position; };
	glm::quat& GetRotationRef() { return m_rotation; };
	glm::vec3& GetScaleRef() { return m_scale; };

	glm::vec3 GetPosition() const { return m_position; };
	glm::quat GetRotation() const { return m_rotation; };
	glm::vec3 GetScale() const { return m_scale; };

protected:

private:

public:
	

protected:

private:
	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::vec3 m_scale;

};

#endif // !_TRANSFORM_H_



