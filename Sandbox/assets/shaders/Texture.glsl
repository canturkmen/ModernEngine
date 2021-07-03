#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;	
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;

out vec2 v_TexCoord;
out vec4 v_Color;

uniform mat4 u_ViewProjectionMatrix;

void main()
{
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform float u_TilingFactor;

void main()
{
	//color = texture(u_Texture, v_TexCoord * u_TilingFactor) * u_Color;
	color = v_Color;
}