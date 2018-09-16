#version 330 core

out vec4 outColor;

in vec3 color;
in vec3 pos;
in vec3 norm;
in vec2 texCoord; //C textura que salen del shader vértices

uniform sampler2D colorTex; //Color
uniform sampler2D emiTex; //Luz emisiva
uniform sampler2D specularTex; //Luz especular
uniform mat4 View;

//Propiedades del objeto
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 N;
float alpha = 1000.0;
vec3 Ke;

//Propiedades de la luz
vec3 Ia = vec3 (0.3); //Ambiental
uniform vec3 Id; //Difusa
vec3 Is = vec3 (1.0); //Especular
uniform vec3 lpos; //Posición de la luz

vec3 shade();

void main()
{
	Ka = texture(colorTex, texCoord).rgb; //Sacado del color de la textura de color
	Kd = texture(colorTex, texCoord).rgb; // ""
	Ke = texture(emiTex, texCoord).rgb; //Sacado de la textura de luz emisiva
	Ks = texture(specularTex, texCoord).rgb; //Luz especular

	N = normalize (norm);
	
	outColor = vec4(shade(), 1.0);   
	
}

vec3 shade()
{
	vec3 c = vec3(0.0);
	c = Ia * Ka;

	vec3 L = normalize (lpos - pos);
	vec3 diffuse = Id * Kd * dot (L,N);
	c += clamp(diffuse, 0.0, 1.0);
	
	vec3 V = normalize (-pos);
	vec3 R = normalize (reflect (-L,N));
	float factor = max (dot (R,V), 0.01);
	vec3 specular = Is*Ks*pow(factor,alpha);
	c += clamp(specular, 0.0, 1.0);

	c+=Ke;
	
	return c;
}
