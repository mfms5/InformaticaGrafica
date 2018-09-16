#version 330 core

//Color de salida
out vec4 outColor;

//Variables Variantes
//Recibe coordenadas de textura
in vec2 texCoord;

//Textura
uniform sampler2D colorTex;


void main()
{
	// 2.23) Se pega la textura en el color de salida
	outColor = vec4(textureLod(colorTex, texCoord,0).xyz, 0.6);	
	//El 0.6 es la transparencia canal alpha). 
	//Si se le quiere dar más peso a la imagen actual, hay que subir ese valor.
	//Si se le quiere dar más peso a la imagen anterior, hay que subir ese valor.
	
	//2.2) Comprobar que las coordenadas de textura son correctas
	//outColor = vec4(texCoord,vec2(1.0));
}