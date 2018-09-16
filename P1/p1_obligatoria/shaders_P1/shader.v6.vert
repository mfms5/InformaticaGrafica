#version 330 core

//Atributo inPos: posición del vértice con el que queremos trabajar
//Todas las uniform y atributos están en shader_allvariables
in vec3 inPos;

//Optimizar, utilizar el color uniforme de shader_allvariables
//6) El color del fragmento viene determinado por el color de los vértices de la primitiva a la que pertenece
in vec3 inColor; //Los colores de cada vértice están definidos en BOX.h

//Las variables globales tienen que estar fuera
//3.3) Mejorar eficiencia del shader
//No es del todo eficiente, hay que unir las matrices y subir a la GPU los valores ya multiplicados
//En shader_allvariables.vert están todas las matrices uniformes
//Se optimiza porque es la CPU la que realiza los cálculos
uniform mat4 modelViewProj;	

//5)Definir variable variante con el color del vértice
out vec3 colorVert;

void main()
{

	colorVert = inColor;

	//Dar valor a la posición
	//Hay que convertir inPos en un vector de 4 para poderlo multiplicar. Se pone 1.0 porque es un punto	
	gl_Position = modelViewProj*vec4 (inPos,1.0);

	
}


