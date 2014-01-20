#version 330

uniform vec4 textColor;
uniform sampler2D fontTexture;

in vec2 textureCoordinates;

out vec4 finalColor;

void main()
{
	finalColor = vec4(1, 1, 1, texture2D(fontTexture, textureCoordinates).a) * textColor;
}