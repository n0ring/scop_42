#shader vertex
#version 330 core
layout(location = 0) in vec2 pos;


void main(){
	gl_Position = vec4(pos, 0.0f, 1.0f);
}


#shader fragment
#version 330 core

out vec4 FragColor;
uniform vec4 our_color;

void main() {
	FragColor = our_color;
}

