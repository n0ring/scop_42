#shader vertex
#version 330 core
layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormCoord;
layout(location = 3) in float aMtlIdx;

out vec4 v_ColorCoord;
uniform mat4 u_MVP;
uniform mat4 u_model;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
flat out float mtlIdx;

vec4 getColorFromPosition(vec4 poss) {
    return vec4(
        0.5 * sin(poss.x) + 0.5,
        0.5 * sin(poss.y) + 0.5,
        0.5 * sin(poss.z) + 0.5,
		1.0f
    );
}

void main(){
	mtlIdx = aMtlIdx;
	gl_Position = u_MVP * pos; 
	TexCoord = aTexCoord; // texture
	v_ColorCoord = getColorFromPosition(normalize(pos));
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
flat in float mtlIdx;

struct Material {
    vec3 ka;      // Ambient Color (12 байт + 4 байта для выравнивания)
    float padding1; // 4 байта для выравнивания
    vec3 kd;      // Diffuse Color (12 байт + 4 байта для выравнивания)
    float padding2; // 4 байта для выравнивания
    vec3 ke;      // Emissive Color (12 байт + 4 байта для выравнивания)
    float padding3; // 4 байта для выравнивания
    vec3 ks;      // Specular Color (12 байт + 4 байта для выравнивания)
	float padding4;
    float Ns;     // Specular Exponent (4 байта)
    float ni;     // Optical Density (4 байта)
    float d;      // Dissolve (Transparency) (4 байта)
    float illum;  // Illumination Model (4 байта)
};


uniform vec4 our_color;
uniform sampler2D u_Texture;
uniform int u_RenderMode;
uniform int u_Light;
uniform vec3 u_lightPos;

layout(std140) uniform Materials {
    Material materials[$materials_size$];
};


  


vec4 getLight(int i, int colorTextMode)
{
	Material material = materials[i];
	vec3 lightColor;
	if (colorTextMode == 0) // color
		lightColor = vec3(v_ColorCoord);
	else // texture
		lightColor = vec3(1.0f);
	vec3 lightPos = u_lightPos;
	vec3 viewPos = vec3(0.0, 0.0, -7.0);
	vec3 objectColor = vec3(v_ColorCoord);
	vec3 diffuseColor = lightColor * vec3(0.5f); 
	vec3 ambientColor = diffuseColor * vec3(0.5f);

    vec3 ambient = ambientColor * material.ka;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * material.kd) ;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Ns);
    vec3 specular = spec * material.ks; 
    vec3 result = ambient + diffuse + specular;
    return vec4(result, material.d);
}


void main() {
	int materialIdx = int(mtlIdx);
	if (u_RenderMode == 0) // color
	{
		if (u_Light == 1)
			color = getLight(materialIdx, u_RenderMode);
		else 
			color = vec4(v_ColorCoord);
	}
	else  // texture
	{
		if (u_Light == 1)
		{
			color = vec4(getLight(materialIdx, u_RenderMode) * vec4(texture(u_Texture, TexCoord)));
		}
		else 
			color = vec4(texture(u_Texture, TexCoord));
	}
}
