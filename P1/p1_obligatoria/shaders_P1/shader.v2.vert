#version 330 core

//Atributo inPos: posición del vértice con el que queremos trabajar
//Todas las uniform y atributos están en shader_allvariables
in vec3 inPos;	

//Las variables globales tienen que estar fuera
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;	

void main()
{
	//Dar valor a la posición
	//Hay que convertir inPos en un vector de 4 para poderlo multiplicar. Se pone 1.0 porque es un punto
	gl_Position = proj*view*vec4 (inPos,1.0);

	
}

