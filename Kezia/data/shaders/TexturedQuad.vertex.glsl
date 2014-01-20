#version 330
uniform mat4 u_ModelViewProjectionMatrix;

in vec3 a_Position;
in vec2 a_TextureCoordinates;

out vec2 textureCoordinates;

void main()
{
	textureCoordinates = a_TextureCoordinates;

	gl_Position = u_ModelViewProjectionMatrix * vec4(a_Position, 1.0);
}