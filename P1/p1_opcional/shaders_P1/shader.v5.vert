#version 330 core

//Atributo inPos: posición del vértice con el que queremos trabajar
//Todas las uniform y atributos están en shader_allvariables
in vec3 inPos;	

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
	//5) Las primitivas pares las pinta azules (lo mismo que en el paso 4, pero en el shader de vértices)
	//Dar valor a colorVert 
	if (mod (gl_VertexID,2)==0) {
		colorVert = vec3(0,0,1);
		}
	else {
		colorVert = vec3 (1);
		}

	//Dar valor a la posición
	//Hay que convertir inPos en un vector de 4 para poderlo multiplicar. Se pone 1.0 porque es un punto	
	gl_Position = modelViewProj*vec4 (inPos,1.0);

	
}

//5) Depurar vértices. Asociar cada uno de los vértices y variar el color de las primitivas según lo cerca que esté de un vértice
//Definir variable variantes (salida del shader vértices (in) y entrada del shader de fragmentos (out))
