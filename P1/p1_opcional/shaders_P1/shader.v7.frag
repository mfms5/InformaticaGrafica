#version 330 core

out vec4 outColor;

//7) Definir variable de entrada con coordenadas de textura
in vec2 texCoord;
//7) Añadir variable uniform
uniform sampler2D colorTex;

void main()
{
	//7) Para acceder a la textura se necesita una función especial
	// Pide el nombre de la textura y la variable con las coordenadas
	outColor = texture (colorTex, texCoord);
	
}
