#version 330 core

in vec3 inPos;	
in vec3 inColor;
in vec3 inNormal; //Atributo de las normales

uniform mat4 modelViewProj;
uniform mat4 normal; //Pasar normales de c. objeto a c. c�mara
uniform mat4 modelView; //Pasar a c. camara

out vec3 color;
out vec3 outPos; //Posici�n en coord c�mara
out vec3 outNormal; //Normales en coord c�mara

//objeto
vec3 ka; //1.4) Coef reflexi�n ambiental
vec3 kd; //1.5) Coef de reflexi�n difusa
vec3 ks; //1.6) Coef refli�n especular
float n = 30; //1.6) Factor que determina el tama�o de la luz

vec3 N; //1.5) Normales, asociadas a los v�rtices, definidas en shader_allvariables
vec3 pos;

//Fuente de luz
vec3 Ia = vec3 (0.3); // 1.4) Luz ambiental
vec3 Il = vec3 (1.0); // 1.5) Luz difusa
vec3 Pl = vec3 (0.0); //1.5) Indica la posici�n de la luz (para calcular vector L), se supone que est�n en c. c�mara
vec3 If = vec3 (0.0); //1.6) Intensidad de la fuente de luz (para brilloes especulares)

vec3 shade();

void main()
{ 
	outPos = vec3(modelView*vec4(inPos, 1.0));
	outNormal = vec3(normal*vec4(inNormal, 0.0));
	color = inColor; //El color (variable salida) se calcula en el m�todo shade
	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}

//Se lleva la funci�n shade a fragmentos, las primeras l�neas del main, y las variables de los objetos y de las fuentes
