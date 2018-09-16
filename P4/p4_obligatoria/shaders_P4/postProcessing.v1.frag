#version 330 core

//Color de salida
out vec4 outColor;

//Variables Variantes
//Recibe coordenadas de textura
in vec2 texCoord;

//Textura
uniform sampler2D colorTex;
//5.1) Textura con los v�rtices donde se va a muestrear la posici�n del v�rtice
uniform sampler2D vertexTex; 

//5.2) 
//Distancia a la que est�s enfocando, negativas xq la c�mara mira a las -Z
const float focalDistance = -25.0; 
//Desenfoque m�ximo
const float maxDistanceFactor = 1.0/5.0;

//PARTE 4.4
//A�adir m�scara de convoluci�n
//Comentar para el 4.5
#define MASK_SIZE 9u
//const float maskFactor = float (1.0/14.0);
uniform float maskFactor;

 vec2 texIdx[MASK_SIZE] = vec2[](
	vec2(-1.0,1.0), vec2(0.0,1.0), vec2(1.0,1.0),
	vec2(-1.0,0.0), vec2(0.0,0.0), vec2(1.0,1.0),
	vec2(-1.0,-1.0), vec2(0.0,-1.0), vec2(1.0,-1.0));

 float mask[MASK_SIZE] = float[](
	float (1.0*maskFactor), float (2.0*maskFactor), float (1.0*maskFactor),
	float (2.0*maskFactor), float (2.0*maskFactor), float (2.0*maskFactor),
	float (1.0*maskFactor), float (2.0*maskFactor), float (1.0*maskFactor)); 

/*
//4.5) Nueva m�scara
#define MASK_SIZE 25u
const vec2 texIdx[MASK_SIZE] = vec2[](
	vec2(-2.0,2.0f), vec2(-1.0,2.0f), vec2(0.0,2.0f), vec2(1.0,2.0f), vec2(2.0,2.0),
	vec2(-2.0,1.0f), vec2(-1.0,1.0f), vec2(0.0,1.0f), vec2(1.0,1.0f), vec2(2.0,1.0),
	vec2(-2.0,0.0f), vec2(-1.0,0.0f), vec2(0.0,0.0f), vec2(1.0,0.0f), vec2(2.0,0.0),
	vec2(-2.0,-1.0f), vec2(-1.0,-1.0f), vec2(0.0,-1.0f), vec2(1.0,-1.0f), vec2(2.0,-1.0),
	vec2(-2.0,-2.0f), vec2(-1.0,-2.0f), vec2(0.0,-2.0f), vec2(1.0,-2.0f), vec2(2.0,-2.0));

const float maskFactor = float (1.0/65.0);
const float mask[MASK_SIZE] = float[](
	1.0*maskFactor, 2.0*maskFactor, 3.0*maskFactor,2.0*maskFactor, 1.0*maskFactor,
	2.0*maskFactor, 3.0*maskFactor, 4.0*maskFactor,3.0*maskFactor, 2.0*maskFactor,
	3.0*maskFactor, 4.0*maskFactor, 5.0*maskFactor,4.0*maskFactor, 3.0*maskFactor,
	2.0*maskFactor, 3.0*maskFactor, 4.0*maskFactor,3.0*maskFactor, 2.0*maskFactor,
	1.0*maskFactor, 2.0*maskFactor, 3.0*maskFactor,2.0*maskFactor, 1.0*maskFactor); */


void main()
{
	//Ser�a m�s r�pido utilizar una variable uniform el tama�o de la textura.
	// Vector de dos 1 dividido entre el tama�o de la textura = factor para acceder al texel m�s cercano
	vec2 ts = vec2(1.0) / vec2 (textureSize (colorTex,0));

	//5.3) A�adir dof
	//1 cuando desenfoque m�ximo, 0 cuando desenfoque m�nimo
	//Dependiendo de si el n�mero tiende a 0 o a 1, tendr� m�s o menos peso el punto central
	//Obtener la distancia focal en funci�n de la textura
	float dof = abs(texture(vertexTex,texCoord).z -focalDistance)* maxDistanceFactor;
	dof = clamp (dof, 0.0, 1.0);
	dof *= dof;
	
	//En principio, el color final va a ser 0 (negro)
	vec4 color = vec4 (0.0);

	//Calcular color de verdad
	for (uint i = 0u; i < MASK_SIZE; i++)
	{
		//iidx tiene la coordenada de textura m�s la posici�n multiplicada por el texel siguiente. 
		//Se utiliza para acceder a la textura de color
		vec2 iidx = texCoord + ts * texIdx[i] * dof; //Calcular �ndice, con el �ndice acceder a la textura
		//dof se a�ade en el paso 5.3
		//Se le suma al color
		color += texture(colorTex, iidx,0.0) * mask[i];
	}
	
	outColor = color;
}



