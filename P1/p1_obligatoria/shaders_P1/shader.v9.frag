#version 330 core

out vec4 outColor;

//9) Variante entrada
in vec3 normalOut; //En el paso 8 era "normal"


void main()
{
	//9
	outColor = vec4(normalOut,1);
}

//9) Las coordenadas de la normal se pasan de c. objeto a c. cámara
