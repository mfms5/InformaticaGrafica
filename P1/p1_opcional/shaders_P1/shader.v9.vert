#version 330 core

//Atributo inPos: posici�n del v�rtice con el que queremos trabajar
//Todas las uniform y atributos est�n en shader_allvariables
in vec3 inPos;

//8) Atributo con las normales
in vec3 inNormal;	

//Las variables globales tienen que estar fuera
//3.3) Mejorar eficiencia del shader
//No es del todo eficiente, hay que unir las matrices y subir a la GPU los valores ya multiplicados
//En shader_allvariables.vert est�n todas las matrices uniformes
//Se optimiza porque es la CPU la que realiza los c�lculos
uniform mat4 modelViewProj;	


//8) Variante de salida con las normales
out vec3 normalOut; //En el paso 8 era "normal"

//9) Variante de entrada, matriz con las normales
uniform mat4 normal; //Matriz de normales en c c�mara

void main()
{

	//9) Dar valor a la salida
	normalOut = (normal*vec4(inNormal,0)).xyz;

		//Dar valor a la posici�n
	//Hay que convertir inPos en un vector de 4 para poderlo multiplicar. Se pone 1.0 porque es un punto	
	gl_Position = modelViewProj*vec4 (inPos,1.0);

	
}

//9) Las coordenadas de la normal se pasan de c. objeto a c. c�mara
