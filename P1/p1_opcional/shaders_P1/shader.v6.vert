#version 330 core

//Atributo inPos: posici�n del v�rtice con el que queremos trabajar
//Todas las uniform y atributos est�n en shader_allvariables
in vec3 inPos;

//Optimizar, utilizar el color uniforme de shader_allvariables
//6) El color del fragmento viene determinado por el color de los v�rtices de la primitiva a la que pertenece
in vec3 inColor; //Los colores de cada v�rtice est�n definidos en BOX.h

//Las variables globales tienen que estar fuera
//3.3) Mejorar eficiencia del shader
//No es del todo eficiente, hay que unir las matrices y subir a la GPU los valores ya multiplicados
//En shader_allvariables.vert est�n todas las matrices uniformes
//Se optimiza porque es la CPU la que realiza los c�lculos
uniform mat4 modelViewProj;	

//5)Definir variable variante con el color del v�rtice
out vec3 colorVert;

void main()
{

	colorVert = inColor;

	//Dar valor a la posici�n
	//Hay que convertir inPos en un vector de 4 para poderlo multiplicar. Se pone 1.0 porque es un punto	
	gl_Position = modelViewProj*vec4 (inPos,1.0);

	
}


