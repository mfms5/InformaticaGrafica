#version 330 core

in vec3 inPos;	
in vec3 inColor;
in vec3 inNormal; //Atributo de las normales
in vec2 inTexCoord; //Coord textura
in vec3 inTangent; //Tangentes

uniform mat4 modelViewProj;
uniform mat4 normal; //Pasar normales de c. objeto a c. cámara
uniform mat4 modelView; //Pasar a c. camara

out vec3 color;
out vec3 outPos; //Posición en coord cámara
out vec3 outNormal; //Normales en coord cámara
out vec2 texCoord; //Salida de las c de textura
out mat3 TBN;

void main()
{ 
	outPos = vec3(modelView*vec4(inPos, 1.0));
	outNormal = vec3(normal*vec4(inNormal, 0.0));
	color = inColor; //El color (variable salida) se calcula en el método shade
	texCoord = inTexCoord; //Dar valor a la salida de las coord textura

	// Matriz TBN, de c tangentes a c cámara 
	vec3 normales = vec3 (normal * normalize(vec4(inNormal, 0.0)));
	vec3 tangentes = vec3 (modelView * normalize(vec4(inTangent, 0.0)));
	vec3 bitangentes = cross (normales, tangentes);
	//Invertir = transponer (ortogonal), de c cámara a c tangentes
	TBN = transpose (mat3 (tangentes, bitangentes, normales));

	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}


