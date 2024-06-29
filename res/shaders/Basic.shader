#shader vertex
#version 330 core
layout(location = 0) in vec4 pos;

out vec4 v_ColorCoord;
uniform mat4 u_MVP;

void main(){
	gl_Position = u_MVP * pos; 

	v_ColorCoord = normalize(pos);
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_ColorCoord;

uniform vec4 our_color;
// uniform sampler2D u_Texture;

void main() {
	// color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	color = v_ColorCoord;
	color[3] = 1.0f;

	// color = vec4(v_ColorCoord, 1.0f);
	// color = vec4(texture(u_Texture, v_TexCoord));
}

