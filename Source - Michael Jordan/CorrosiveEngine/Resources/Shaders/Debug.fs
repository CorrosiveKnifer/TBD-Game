#version 450 core

out vec4 color;

uniform vec4 colorInput;

void main()
{
	color = colorInput;
}