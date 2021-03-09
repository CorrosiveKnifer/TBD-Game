#ifndef _LABEL_H_
#define _LABEL_H_

#include "Widget.h"

#include "TextGenerator.h"

class Label : public Widget
{
	//Member Functions:
public:
	Label(std::string _text, std::string _font, int _size);
	virtual ~Label();

	virtual void Draw(Program* _program);
	virtual void Process(float dT);

	void SetText(std::string _text) { m_text = _text; };
	void SetColour(glm::vec3 _col) { m_colour = _col; };
protected:
	virtual bool CheckMousePosition(glm::vec2 _mouse);

private:

	//Member Data:
public:

protected:
	std::string m_text;
	TextGenerator* m_generator;
	glm::vec3 m_colour;
private:

};

#endif // !_LABEL_H_
