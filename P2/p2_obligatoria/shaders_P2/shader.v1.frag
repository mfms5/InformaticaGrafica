#version 330 core

out vec4 outColor;

//Recibe el color de la etapa de v�rtices
in vec3 color;

void main()
{
	outColor = vec4(color, 1.0);   //Lo pone a la salida
}
