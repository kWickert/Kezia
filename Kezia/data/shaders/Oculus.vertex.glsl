#version 330
in vec3 a_Position;
in vec2 a_TextureCoordinates;

out vec2 TextureCoordinates;

void main()
{
	TextureCoordinates = a_TextureCoordinates;
	
	gl_Position = vec4(a_Position, 1.0);
}