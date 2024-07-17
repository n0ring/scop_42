#shader vertex
#version 330 core
layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormCoord;

out vec4 v_ColorCoord;
uniform mat4 u_MVP;
uniform mat4 u_model;
out vec2 TexCoord;
out vec3 FragPos;    // Позиция фрагмента
out vec3 Normal;     // Нормаль фрагмента



vec4 getColorFromPosition(vec4 poss) {
    return vec4(
        0.5 * sin(poss.x) + 0.5,
        0.5 * sin(poss.y) + 0.5,
        0.5 * sin(poss.z) + 0.5,
		1.0f
    );
}

void main(){
	gl_Position = u_MVP * pos; 
	TexCoord = aTexCoord; // texture
	v_ColorCoord = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	// Позиция и нормаль для фрагментного шейдера
    FragPos = vec3(u_model * pos);
    Normal = mat3(transpose(inverse(u_model))) *  normalize(aNormCoord);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_ColorCoord;
in vec2 TexCoord;

in vec3 FragPos;    // Позиция фрагмента, интерполированная из вершинного шейдера
in vec3 Normal;     // Нормаль фрагмента, интерполированная из вершинного шейдера

uniform vec4 our_color;
uniform sampler2D u_Texture;
uniform int u_RenderMode;
uniform int u_HasNormal;
uniform int u_Light;


void main() {
	if (u_Light == 1) // just to remove error
	{}
	if (u_RenderMode == 0) // color
	{
		if (u_HasNormal == 1)
			color = vec4(v_ColorCoord);
		else 
			color = vec4(v_ColorCoord);
	}
	else  // texture
	{
		if (u_HasNormal == 1)
			color = vec4(texture(u_Texture, TexCoord));
		else 
			color = vec4(texture(u_Texture, TexCoord));
	}
}



// #shader vertex
// #version 330 core
// layout(location = 0) in vec4 pos;
// layout(location = 1) in vec2 aTexCoord;

// out vec4 v_ColorCoord;
// uniform mat4 u_MVP;
// out vec2 TexCoord;

// vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);
// vec4 yellow = vec4(1.0, 1.0, 0.0, 1.0);

// vec4 getColorFromPosition(vec4 poss) {
//     // Normalize the y coordinate to be between 0 and 1
//     float normalizedY = (poss.y + 1.0) * 0.5;
//     // Interpolate between blue and yellow
//     return mix(blue, yellow, normalizedY);
// }

// void main(){
//     gl_Position = u_MVP * pos; 
//     TexCoord = aTexCoord; // texture
//     v_ColorCoord = getColorFromPosition(normalize(pos));
// }


// wbw
// #shader vertex
// #version 330 core
// layout(location = 0) in vec4 pos;
// layout(location = 1) in vec2 aTexCoord;

// out vec4 v_ColorCoord;
// uniform mat4 u_MVP;
// out vec2 TexCoord;

// vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
// vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);

// vec4 getColorFromPosition(vec4 poss) {
//     float normalizedY = (poss.y + 1.0) * 0.5;

//     if (normalizedY < 0.5) {
//         return mix(white, blue, normalizedY / 0.5);
//     } else {
//         return mix(blue, white, (normalizedY - 0.5) / 0.5);
//     }
// }

// void main() {
//     gl_Position = u_MVP * pos;
//     TexCoord = aTexCoord; // texture
//     v_ColorCoord = getColorFromPosition(normalize(pos));
// }

