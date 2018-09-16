//AÑADIR UNA FUENTE DE LUZ MÁS
//ATENUACIÓN
//LUZ DIRECCIONAL
//LUZ FOCAL

#version 330 core

out vec4 outColor;

//Variantes
in vec3 color; //Recibe el color de la etapa de vértices
in vec3 outNormal; //Recibe normales en c camara
in vec3 outPos; //Recibe posición en c camara
in vec2 texCoord; //Recibe coordenaadas de textura

//Uniformes
uniform sampler2D colorTex; //Textura
uniform sampler2D emiTex; //Textura luz emisiva
uniform sampler2D specularTex; //Textura luz especular

//objeto
vec3 ka; //1.4) Coef reflexión ambiental
vec3 kd; //1.5) Coef de reflexión difusa
vec3 ks; //1.6) Coef reflexión especular
vec3 ks2; //Coef reflexión especular para la segunda fuente
float n = 30; //1.6) Factor que determina el tamaño de la luz (>, luz más pequeña) //Se cambia de 300 a 30 en luz espec
vec3 ke; //Coef luz emisiva
vec3 N; //1.5) Normales, asociadas a los vértcices, definidas en shader_allvariables
vec3 pos; //Posición del objeto


//Fuente de luz 1
vec3 Ia = vec3 (0.3); // 1.4) Luz ambiental
vec3 Il = vec3 (1.0); // 1.5) Luz difusa
vec3 Pl = vec3 (0.0, 0.0, 6.0); //3.7)Cambiar posición luz para que se vea la luz especular de la textura
vec3 If = vec3 (0.0); //1.6) Intensidad de la fuente de luz (para brillos especulares)
float fatt1; //atenuación según la distancia

//Fuente de luz 2
vec3 Pl2 = vec3 (10.0, 0.0, -30.0); //Posición de la segunda fuente de luz, se sitúa a la derecha de la cámara, se ve por el lateral del cubo
float fatt2; //atenuación según la distancia
//Resto de parámetros iguales que los de la otra luz

//Luz direccional
vec3 Pld = vec3 (-3.0, 10.0, 1.0); //Dirección rayos
//Resto de parámetros iguales que los de la otra luz, no necesita atenuación

//Luz focal
vec3 Plf = vec3 (0.0, 0.0, 1.0); //Posición
float fattf; //Atenuación
vec3 dir = normalize(vec3 (0.0, 0.0, -1.0)); //Dirección foco
float angulo = 0.2; //Ángulo de corte
float penumbra = 0.25; //Ángulo penumbra
//Resto de parámetros iguales

//Constantes atenuación
float c1 = 1.0;
float c2 = 0.03;
float c3 = 0.0;

vec3 shade();

void main()
{
    ka = texture (colorTex, texCoord).rgb; //Se le da el valor de la textura (nombre, coordenadas);
	kd = texture (colorTex, texCoord).rgb; //Lo mismo para el cr difusa
	ks = texture (specularTex, texCoord).rgb; //3.7) Textura especular
	ks2 = vec3 (0.0, 0.0, 1.0); // Brillo especular de la segunda fuente, azul
	ke = texture (emiTex, texCoord).rgb; //Dar valor a ke con la textura emisiva y las mismas coord textura que antes (podrían ser distintas)

	N = normalize (outNormal); //Normalizar normales
	
	pos = outPos; 

	//Calcular Fatt
	float d1 = length(Pl-pos); //Distancia luz 1 - objeto
	fatt1 = min(1/(c1+c2*d1+c3*d1*d1), 1);
	float d2 = length(Pl2-pos); //Distancia luz 2 - objeto
	fatt2 = min(1/(c1+c2*d2+c3*d2*d2), 1);

	outColor = vec4(shade(), 1.0);   //Lo pone a la salida
}

vec3 shade() 
{
	//ILUMINACIÓN AMBIENTAL (igual para todos las fuentes de luz)
	vec3 color = vec3(0.0); //Todos los fragmentos negros
	color = Ia*ka; //Le añade la intensidad de la luz ambiental (pequeña) y el coeficiente

	//ATENUACIÓN SI ES LUZ FOCAL
	vec3 Lf = normalize(Plf-pos); //Vector superficie-luz
	vec3 rayoLuz = -Lf; //Vector luz-superficie
	float anguloRayo = acos(dot(rayoLuz,dir));
	if (anguloRayo > angulo) {
		if(anguloRayo <= penumbra) {
		fattf = (penumbra-anguloRayo)*0.5; //Dentro de la zona de penumbra, poca luz
		}
		else {
		fattf = 0.0; //Zona de oscuridad, no se ve el rayo
		}
    }
	else {
		fattf =(angulo-anguloRayo)*10; //Zona iluminada, atenuación normal
	}

	//ILUMINACIÓN DIFUSA
	//Luz 1
	vec3 L = normalize (Pl - pos); //Extremo (posición luz, la misma que en la cámara) - origen (posición punto), normalizado --> Vector superficie-luz
	float prod = clamp(dot(N,L), 0.0001, 1.0);
	//Luz 2
	vec3 L2 = normalize (Pl2 - pos); //Segunda fuente de luz
	float prod2 = clamp (dot(N,L2), 0.0001, 1.0);
	//Luz direccional
	vec3 Ld = normalize(Pld); //La luz direccional sólo necesita la dirección de los rayos de luz	
	float prodd = clamp (dot(N,Ld), 0.0001, 1.0); 
	//Luz focal
	float prodf = clamp (dot(N,Lf), 0.0001, 1.0);
	//Total
	color += (Il*kd)*(prod*fatt1+prod2*fatt2+prodd+prodf*fattf); //Al color se le añade la luz difusa = intensidad+cr difusa+cos N^L (prod escalar), con atenuación
	
	//ILUMINACIÓN ESPECULAR
	vec3 V = normalize(-pos); //1.6) Vector de posición del observador (-posición el punto en c camara)
	//Luz 1
	vec3 R = normalize(reflect (-L,N)); //1.6) Refleja el vector -L sobre el vector N
	float factor = clamp (dot(V,R), 0.0001, 1.0); //1.6) Prod escalar V*R --> Lo ajusta con clamp: dot<min, coge min; dot>max, coge max; dot entre min y max, coge dot
	factor = pow (factor, n); //1.6) (V*R)^n 
	//Luz 2
	vec3 R2 = normalize(reflect(-L2,N));
	float factor2 = clamp (dot(V,R2), 0.0001, 1.0);
	factor2 = pow (factor2, n);
	//Luz direccional
	vec3 Rd = normalize(reflect(-Ld,N));	
	float factord = clamp (dot(V,Rd), 0.0001, 1.0);	
	factord = pow (factord, n); 
	//Luz focal
	vec3 Rf = normalize(reflect(-Lf,N));
	float factorf = clamp(dot(V,Rf), 0.0001, 1.0);
	factorf = pow (factorf, n);
	//Total
	color += If*(ks*factor*fatt1 + ks2*factor2*fatt2 + ks*factord + ks*factorf*fattf); //Añade luz especular, con atenuación

	//ILUMINACIÓN EMISIVA (igual para todas las fuentes)
	color += ke; //Luz emisiva (las letras de la textura "brillan en la oscuridad")
	
	return color;
}
