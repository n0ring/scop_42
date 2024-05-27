#shader vertex
#version 330 core

in vec4 position;

void main(){
gl_Position = position;
}


#shader fragment
#version 330 core

out vec4 FragColor;

void main(){
	FragColor = vec4(0.2f, 0.3f, 0.8f, 1.0f);
}