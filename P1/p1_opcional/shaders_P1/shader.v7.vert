#version 330 core

//Atributo inPos: posición del vértice con el que queremos trabajar
//Todas las uniform y atributos están en shader_allvariables
in vec3 inPos;

//7) Atributo de coordenadas de textura para cada vértice que está en allvariables
in vec2 inTexCoord; //Las coordenadas de textura están definidas en BOX.h

//Las variables globales tienen que estar fuera
//3.3) Mejorar eficiencia del shader
//No es del todo eficiente, hay que unir las matrices y subir a la GPU los valores ya multiplicados
//En shader_allvariables.vert están todas las matrices uniformes
//Se optimiza porque es la CPU la que realiza los cálculos
uniform mat4 modelViewProj;	

//7) Definir variable de salida con coordenadas de textura
out vec2 texCoord;

void main()
{
	//Dar valor a la variable de coord textura. Se necesita un atributo asociado a un vértice que de coord textura (en BOX y allvariables)
	texCoord = inTexCoord;

	//Dar valor a la posición
	//Hay que convertir inPos en un vector de 4 para poderlo multiplicar. Se pone 1.0 porque es un punto	
	gl_Position = modelViewProj*vec4 (inPos,1.0);

	
}

