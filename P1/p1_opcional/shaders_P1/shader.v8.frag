#version 330 core

out vec4 outColor;

//8) Variante entrada
in vec3 normal;

void main()
{
	//8)Dar valor a la salida
	outColor = abs(vec4(normal,1)); //Se pone el valor absoluto para ver las normales negativas
	
}

//8) Pintar las normales por pantalla con un color cada una
