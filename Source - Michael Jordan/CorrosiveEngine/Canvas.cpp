#include "Canvas.h"

Canvas::Canvas()
{
	m_program = &Renderer::GetInstance().CreateProgram("Widget.vs", "Widget.fs");
}

Canvas::~Canvas()
{
	std::map<std::string, Item>::iterator iter = m_items.begin();

	while (iter != m_items.end())
	{
		delete (*iter).second.widget;
		iter = m_items.erase(iter);
	}
}

void Canvas::AddWidget(std::string _key, Widget* _widget, glm::vec2 _localAlign, glm::vec2 _canvasAlign, glm::vec2 _pos)
{
	Item i;

	_widget->SetLocalPosition(_pos);
	_widget->SetLocalAlignment(_localAlign);
	_widget->SetCanvasAlignment(Canvas::GetAlignment(_canvasAlign));

	i.widget = _widget;
	i.alignment = _canvasAlign;

	m_items.insert(std::pair<std::string, Item>(_key, i));
}

std::string Canvas::GetWidgetKeyWithValue(float _value)
{
	std::map<std::string, Item>::iterator iter = m_items.begin();

	while (iter != m_items.end())
	{
		if ((*iter).second.widget->GetValue() == _value)
		{
			return (*iter).first;
		}
		iter++;
	}
}

void Canvas::Process(float dt)
{
	std::map<std::string, Item>::iterator iter = m_items.begin();

	while (iter != m_items.end())
	{
		(*iter).second.widget->Process(dt);
		iter++;
	}
}

void Canvas::Draw()
{
	std::map<std::string, Item>::iterator iter = m_items.begin();
	
	while (iter != m_items.end())
	{
		(*iter).second.widget->Draw(m_program);
		iter++;
	}
}

glm::vec2 Canvas::GetAlignment(glm::vec2 _alignment)
{
	glm::vec2 result;

	result.x = _alignment.x * Renderer::SCREEN_WIDTH;
	result.y = _alignment.y * Renderer::SCREEN_HEIGHT;

	return result;
}

void Canvas::Resize()
{
	std::map<std::string, Item>::iterator iter = m_items.begin();

	while (iter != m_items.end())
	{
		glm::vec2 newAlign = GetAlignment((*iter).second.alignment);
		(*iter).second.widget->SetCanvasAlignment(newAlign);
		iter++;
	}
}
