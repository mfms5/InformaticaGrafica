#version 330 core

//Atributo inPos: posici�n del v�rtice con el que queremos trabajar
//Todas las uniform y atributos est�n en shader_allvariables
in vec3 inPos;	

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
	//5) Las primitivas pares las pinta azules (lo mismo que en el paso 4, pero en el shader de v�rtices)
	//Dar valor a colorVert 
	if (mod (gl_VertexID,2)==0) {
		colorVert = vec3(0,0,1);
		}
	else {
		colorVert = vec3 (1);
		}

	//Dar valor a la posici�n
	//Hay que convertir inPos en un vector de 4 para poderlo multiplicar. Se pone 1.0 porque es un punto	
	gl_Position = modelViewProj*vec4 (inPos,1.0);

	
}

//5) Depurar v�rtices. Asociar cada uno de los v�rtices y variar el color de las primitivas seg�n lo cerca que est� de un v�rtice
//Definir variable variantes (salida del shader v�rtices (in) y entrada del shader de fragmentos (out))
