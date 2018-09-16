// AÑADIR DISTINTOS TIPOS DE LUCES
//BUMP MAPPING

#version 330 core

out vec4 outColor;

in vec3 color;
in vec3 pos;
in vec3 norm;
in vec2 texCoord; //C textura que salen del shader vértices
in mat3 TBN; //Matriz transpuesta TBN

uniform sampler2D colorTex; //Color
uniform sampler2D emiTex; //Luz emisiva
uniform sampler2D specularTex; //Luz especular
uniform sampler2D normalTex; //Mapa normales
uniform mat4 View;

//Propiedades del objeto
vec3 Ka; //Ambiental
vec3 Kd; //Difusa
vec3 Ks; //Especular
vec3 N; //Normales
float alpha = 5000.0; //Brillo
vec3 Ke; //Emisiva

//Fuente 1
vec3 Ia = vec3 (0.3); //Ambiental
uniform vec3 Id; //Difusa
vec3 Is = vec3 (1.0); //Especular
uniform vec3 lpos; 
float fatt1;

//Luz direccional
vec3 Pld = vec3 (-3.0,10.0,1.0); //Dirección rayos
//Resto de parámetros iguales a los de la Luz 1

//Luz focal
vec3 Plf = vec3(0.0,0.0,1.0); //Posición
float fattf; //Atenuación
vec3 dirFoco = normalize(vec3(0.0,0.0,-1.0)); //Dirección foco
float anguloCorte = 0.2; //Ángulo de corte
float anguloPenum = 0.25; //Ángulo penumbra
//Resto de parámetros iguales a los de la Luz 1

//Constantes atenuación
float c1 = 1.0;
float c2 = 0.03;
float c3 = 0.0;

vec3 shade();

void main()
{
	Ka = texture(colorTex, texCoord).rgb; //Sacado del color de la textura de color
	Kd = texture(colorTex, texCoord).rgb; // ""
	Ke = texture(emiTex, texCoord).rgb; //Sacado de la textura de luz emisiva
	Ks = texture(specularTex, texCoord).rgb; //Luz especular

	//N = normalize (norm);
	//Sustituir normales por las del mapa
	N = normalize (texture (normalTex, texCoord).rgb*2.0-1.0);

	//Calcular Fatt luz 1
	float d1 = length(lpos-pos); //Distancia luz 1 - objeto
	fatt1 = min(1/(c1+c2*d1+c3*d1*d1), 1);
	
	outColor = vec4(shade(), 1.0);   
	
}

vec3 shade()
{
	//AMBIENTAL
	vec3 c = vec3(0.0);
	c = Ia * Ka;

	//ATENUACIÓN LUZ FOCAL
	vec3 Lf = normalize(Plf-pos); //Vector superficie-luz
	Lf = TBN*Lf;
	vec3 rayoLuz = -Lf; //Vector luz-superficie
	float anguloRayo = acos(dot(rayoLuz,dirFoco));
	if (anguloRayo > anguloCorte) {
		if(anguloRayo <= anguloPenum) {
		fattf = (anguloPenum-anguloRayo)*0.5; //Dentro de la zona de penumbra, poca luz
		}
		else {
		fattf = 0.0; //Zona de oscuridad, no se ve el rayo
		}
    }
	else {
		fattf =(anguloCorte-anguloRayo)*10; //Zona iluminada, atenuación normal
	}

	//DIFUSA
	//Luz 1
	vec3 L = normalize (lpos - pos);
	L = TBN*L;
	float prod = clamp(dot(N,L), 0.0001, 1.0);
	//Luz direccional
	vec3 Ld = normalize(Pld);
	Ld = TBN*Ld;
	float prodd = clamp (dot(N,Ld), 0.0001, 1.0); 
	//Luz focal
	float prodf = clamp (dot(N,Lf), 0.0001, 1.0);
	//Total
	vec3 diffuse = Id*Kd*(prod*fatt1+prodd+prodf*fattf);
	c += clamp(diffuse, 0.0, 1.0);

	//ESPECULAR	
	vec3 V = normalize (-pos);
	V = TBN*V;
	//Luz 1
	vec3 R = normalize (reflect (-L,N));
	float factor = max (dot (R,V), 0.01);
	factor = pow (factor, alpha);
	//Luz direccional
	vec3 Rd = normalize(reflect(-Ld,N));	
	float factord = max (dot (Rd,V), 0.01);	
	factord = pow (factord, alpha);
	//Luz focal
	vec3 Rf = normalize(reflect(-Lf,N));
	float factorf =  max (dot (Rf,V), 0.01);
	factorf = pow (factorf, alpha);
	//Total
	vec3 specular = Is*Ks*(factor*fatt1+factord+fattf*factorf);
	c += clamp(specular, 0.0, 1.0);

	//EMISIVA
	c+=Ke;
	
	return c;
}
