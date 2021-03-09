#include "Label.h"

#include "InputHandler.h"

Label::Label(std::string _text, std::string _font, int _size)
{
    m_text = _text;
    m_colour = glm::vec3(0.0f, 0.0f, 0.0f);

    m_generator = &Renderer::GetInstance().CreateTextGenerator(_font, _size);
}

Label::~Label()
{

}

void Label::Draw(Program* _program)
{
	glm::vec2 scale = GetLocalScale();
	glm::vec2 size = m_generator->GetDimentionsOfText(m_text);

	glm::vec2 topLeft = glm::vec2(
		(m_alignCanvas.x + GetLocalPosition().x) - (m_alignLocal.x * size.x * scale.x),
		(m_alignCanvas.y + GetLocalPosition().y) - (m_alignLocal.y * size.y * scale.y)
	);

	m_generator->SetColour(m_colour);
	m_generator->RenderText(m_text, glm::vec2(topLeft.x, topLeft.y));
}

void Label::Process(float dT)
{
	if (CheckMousePosition(InputHandler::GetInstance().GetMousePos()))
	{
		if (InputHandler::GetInstance().IsMousePressed(GLUT_LEFT_BUTTON))
		{
			if (m_mouseClickCallBack != NULL)
				m_mouseClickCallBack();
		}
	}
}

bool Label::CheckMousePosition(glm::vec2 _mouse)
{
	float mouseX = _mouse.x;
	float mouseY = Renderer::SCREEN_HEIGHT - _mouse.y;

	glm::vec2 size = m_generator->GetDimentionsOfText(m_text) * 0.5f;

	float width = size.x * GetLocalScale().x;
	float height = size.y * GetLocalScale().y;

	glm::vec2 topLeft = glm::vec2(
		(m_alignCanvas.x + GetLocalPosition().x) - (m_alignLocal.x * width),
		(m_alignCanvas.y + GetLocalPosition().y) - ((1.0f - m_alignLocal.y) * height)
	);

	bool xAxis = (mouseX > topLeft.x) && (mouseX < topLeft.x + width);
	bool yAxis = (mouseY > topLeft.y) && (mouseY < topLeft.y + height);

	return xAxis && yAxis;
}
