#shader vertex
#version 330 core
layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 aTexCoord;

out vec4 v_ColorCoord;
uniform mat4 u_MVP;
out vec2 TexCoord;

void main(){
	gl_Position = u_MVP * pos; 
	TexCoord = aTexCoord; // texture
	v_ColorCoord = normalize(pos);
	v_ColorCoord[3] = 1.0f;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_ColorCoord;
in vec2 TexCoord;

uniform vec4 our_color;
uniform sampler2D u_Texture;
uniform int u_RenderMode;
void main() {

	if (u_RenderMode == 0)
	{
		color = vec4(v_ColorCoord);
	}
	else 
	{
		color = vec4(texture(u_Texture, TexCoord));
	}
}

