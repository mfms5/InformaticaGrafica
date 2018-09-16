#version 330 core

out vec4 outColor;

//Variantes
in vec3 color; //Recibe el color de la etapa de v�rtices
in vec3 outNormal; //Recibe normales en c camara
in vec3 outPos; //Recibe posici�n en c camara
in vec2 texCoord; //Recibe coordenaadas de textura

//Uniformes
uniform sampler2D colorTex; //Textura
uniform sampler2D emiTex; //Textura luz emisiva
uniform sampler2D specularTex; //Textura luz especular

//objeto
vec3 ka; //1.4) Coef reflexi�n ambiental
vec3 kd; //1.5) Coef de reflexi�n difusa
vec3 ks; //1.6) Coef refli�n especular
float n = 30; //1.6) Factor que determina el tama�o de la luz (>, luz m�s peque�a) //Se cambia de 300 a 30 en luz espec
vec3 ke; //Coef luz emisiva
vec3 N; //1.5) Normales, asociadas a los v�rtcices, definidas en shader_allvariables
vec3 pos;

//Fuente de luz
vec3 Ia = vec3 (0.3); // 1.4) Luz ambiental
vec3 Il = vec3 (1.0); // 1.5) Luz difusa
//vec3 Pl = vec3 (0.0); //1.5) Indica la posici�n de la luz (para calcular vector L), se supone que est�n en c. c�mara
vec3 Pl = vec3 (0.0, 0.0, 6.0); //3.7)Cambiar posici�n luz para que se vea la luz especular de la textura
vec3 If = vec3 (0.0); //1.6) Intensidad de la fuente de luz (para brillos especulares)

vec3 shade();


void main()
{

    ka = texture (colorTex, texCoord).rgb; //Se le da el valor de la textura (nombre, coordenadas);
	kd = texture (colorTex, texCoord).rgb; //Lo mismo para el cr difusa
	ks = texture (specularTex, texCoord).rgb; //3.7) Textura especular
	//ks = vec3 (1.0); //1.6) Brillo especular blanco
	ke = texture (emiTex, texCoord).rgb; //Dar valor a ke con la textura emisiva y las mismas coord textura que antes (podr�an ser distintas)

	N = normalize (outNormal); //Normalizar normales

	pos = outPos; //Pasar coordenadas del punto inPos (c. objeto) a c. c�mara

	outColor = vec4(shade(), 1.0);   //Lo pone a la salida
}

vec3 shade() 
{
	vec3 color = vec3(0.0); //Todos los fragmentos negros
	color = Ia*ka; //Le a�ade la intensidad de la luz ambiental (peque�a) y el coeficiente

	vec3 L = normalize (Pl - pos); //Extremo (posici�n luz, la misma que en la c�mara) - origen (posici�n punto), normalizado
	float prod = clamp(dot(N,L), 0.0001, 1.0);
	color += Il*kd*prod; //Al color se le a�ade la luz difusa = intensidad+cr difusa+cos N^L (prod escalar)
	
	vec3 V = normalize(-pos); //1.6) Vector de posici�n del observador (-posici�n el punto en c camara)
	vec3 R = normalize(reflect (-L,N)); //1.6) Refleja el vector -L sobre el vector N
	float factor = clamp (dot(V,R), 0.0001, 1.0); //1.6) Prod escalar V*R --> Lo ajusta con clamp: dot<min, coge min; dot>max, coge max; dot entre min y max, coge dot
	factor = pow (factor, n); //1.6) (V*R)^n 
	color += Il * ks * factor; //A�ade luz especular

	color += ke; //Luz emisiva (las letras de la textura "brillan en la oscuridad")
	
	return color;
}

/*Dejar luz fija en la c�mara
uniform mat4 view;

vec3 L = normalize (vec3(view*vec4(P1,1)-pos); */