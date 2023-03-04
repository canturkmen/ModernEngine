#type vertex
#version 450 core
			
layout(location = 0) in vec3 a_Position;	
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 5) in int a_EntityID;

out vec2 v_TexCoord;
out vec4 v_Color;
out flat int v_EntityID;

uniform mat4 u_ViewProjectionMatrix;

void main()
{
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

in vec2 v_TexCoord;
in vec4 v_Color;
in flat int v_EntityID;

layout(binding = 0) uniform sampler2D u_FontAtlasTexture;

void main()
{
	vec4 texColor = v_Color * texture(u_FontAtlasTexture, v_TexCoord);

	if(texColor.a == 0.0)
		discard;

	color = texColor;
	color2 = v_EntityID;
}