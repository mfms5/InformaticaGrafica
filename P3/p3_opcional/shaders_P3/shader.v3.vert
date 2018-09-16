//BUMP MAPPING

#version 330 core

in vec3 inPos;	
in vec3 inColor;
in vec2 inTexCoord;
in vec3 inNormal;
in vec3 inTangent; //Tangentes

uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;

out vec3 color;
out vec3 pos;
out vec3 norm;
out vec2 texCoord;
out mat3 TBN;

void main()
{
	color = inColor;
	texCoord = inTexCoord;
	norm = (normal * vec4(inNormal, 0.0)).xyz;
	pos = (modelView * vec4(inPos, 1.0)).xyz;
	
	// Matriz TBN, de c tangentes a c c�mara 
	vec3 normales = vec3 (normal * normalize(vec4(inNormal, 0.0)));
	vec3 tangentes = vec3 (modelView * normalize(vec4(inTangent, 0.0)));
	vec3 bitangentes = cross (normales, tangentes);
	//Invertir = transponer (ortogonal), de c c�mara a c tangentes
	TBN = transpose (mat3 (tangentes, bitangentes, normales));

	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}
