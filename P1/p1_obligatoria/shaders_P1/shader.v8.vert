#version 330 core

//Atributo inPos: posición del vértice con el que queremos trabajar
//Todas las uniform y atributos están en shader_allvariables
in vec3 inPos;

//8) Atributo con las normales
in vec3 inNormal;	

//Las variables globales tienen que estar fuera
//3.3) Mejorar eficiencia del shader
//No es del todo eficiente, hay que unir las matrices y subir a la GPU los valores ya multiplicados
//En shader_allvariables.vert están todas las matrices uniformes
//Se optimiza porque es la CPU la que realiza los cálculos
uniform mat4 modelViewProj;	

//8) Variante de salida con las normales
out vec3 normal; //Definidas en BOX.h

void main()
{
	//8) Dar valor a la variante
	normal = inNormal;
	
	//Dar valor a la posición
	//Hay que convertir inPos en un vector de 4 para poderlo multiplicar. Se pone 1.0 porque es un punto	
	gl_Position = modelViewProj*vec4 (inPos,1.0);

	
}

//8) Pintar las normales por pantalla con un color cada una
// Tiene que haber en allvariable un atributo donde esten las normales (inNormal)
//Las coordenadas de las normales están en coordenadas del objeto, no valen :( En el paso 9 se pasan a cámara

