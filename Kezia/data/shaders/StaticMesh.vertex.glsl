#version 330
uniform mat4 u_ViewProjection;

in vec3 a_Position;
in vec3 a_Normal;
in vec2 a_TextureCoordinates;

in mat4 a_Model;

out vec2 v_TextureCoordinates;
out vec3 v_Normal;

void main()
{
	v_TextureCoordinates = a_TextureCoordinates;
	v_Normal = a_Normal;
	
	gl_Position = u_ViewProjection * a_Model * vec4(a_Position, 1.0);
}