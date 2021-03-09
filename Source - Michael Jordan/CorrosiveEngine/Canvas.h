#ifndef _CANVAS_H_
#define _CANVAS_H_

#include "Widget.h"
#include "Program.h"

#include <map>

struct Item
{
	Widget* widget;
	glm::vec2 alignment;
};

class Canvas
{
public:
	Canvas();
	~Canvas();

	void AddWidget(std::string _key, Widget* _widget, glm::vec2 _localAlign, glm::vec2 _canvasAlign, glm::vec2 _pos);
	
	void Process(float dt);
	void Draw();

	void Resize();

	static glm::vec2 GetAlignment(glm::vec2 _alignment);
	std::string GetWidgetKeyWithValue(float _value);
	Widget* GetWidget(std::string _key) { return m_items[_key].widget; };
protected:

private:

public:

protected:
	std::map<std::string, Item> m_items;
	Program* m_program;

private:

};

#endif