#shader vertex
#version 330 core
layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;
uniform mat4 u_MVP;

void main(){
	gl_Position = u_MVP * pos; 
	v_TexCoord = texCoord;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 our_color;
uniform sampler2D u_Texture;

void main() {
	color = vec4(texture(u_Texture, v_TexCoord));
	color = vec4(1.0);
}

