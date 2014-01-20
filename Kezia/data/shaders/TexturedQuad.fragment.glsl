#version 330

uniform vec4 u_QuadColor;
uniform sampler2D u_QuadTexture;

in vec2 textureCoordinates;

out vec4 finalColor;

void main()
{
	finalColor = texture2D(u_QuadTexture, textureCoordinates) * u_QuadColor;
}