#version 330
uniform sampler2D u_DiffuseTexture;

out vec2 v_TextureCoordinates;

void main()
{
	gl_Color = texture2D(u_DiffuseTexture, v_TextureCoordinates);
}