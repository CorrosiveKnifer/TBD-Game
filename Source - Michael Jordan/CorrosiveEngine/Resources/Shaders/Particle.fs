#version 450 core

in vec4 FragColor;
in vec2 FragTexCoords;

out vec4 FinalColor;

uniform sampler2D ImageTexture;

void main()
{
	FinalColor = texture(ImageTexture, FragTexCoords) * FragColor;
}