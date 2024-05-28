#shader vertex
#version 330 core
layout(location = 0) in vec2 pos;
layout(location = 1) in vec3 color;

out vec4 outColor;

void main(){
	gl_Position = vec4(pos, 0.0f, 1.0f);
	outColor = vec4(color, 1.0f);
}


#shader fragment
#version 330 core
in vec4 outColor;
out vec4 FragColor;


void main() {
	FragColor = outColor;
}

