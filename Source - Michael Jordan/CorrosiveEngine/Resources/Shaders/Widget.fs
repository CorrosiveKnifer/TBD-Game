#version 450 core

in vec2 FragTexCoords;

uniform sampler2D ImageTexture;

out vec4 FinalColor;

void main()
{
	FinalColor = texture(ImageTexture, FragTexCoords);
}