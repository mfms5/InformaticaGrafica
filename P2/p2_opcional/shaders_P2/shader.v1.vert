#version 330 core

in vec3 inPos;	
in vec3 inColor;
in vec3 inNormal; //Atributo de las normales

uniform mat4 modelViewProj;
uniform mat4 normal; //Pasar normales de c. objeto a c. cámara
uniform mat4 modelView; //Pasar a c. camara
out vec3 color;

//objeto
vec3 ka; //1.4) Coef reflexión ambiental
vec3 kd; //1.5) Coef de reflexión difusa
vec3 ks; //1.6) Coef reflión especular
float n = 30; //1.6) Factor que determina el tamaño de la luz

vec3 N; //1.5) Normales, asociadas a los vértcices, definidas en shader_allvariables
vec3 pos;

//Fuente de luz
vec3 Ia = vec3 (0.3); // 1.4) Luz ambiental
vec3 Il = vec3 (1.0); // 1.5) Luz difusa
vec3 Pl = vec3 (0.0); //1.5) Indica la posición de la luz (para calcular vector L), se supone que están en c. cámara
vec3 If = vec3 (0.0); //1.6) Intensidad de la fuente de luz (para brillos especulares)

vec3 shade();

void main()
{
	ka = inColor; //1.4) El cr ambiental depende del del color de entrada (color del objeto)
	kd = inColor; //Lo mismo para el cr difusa
	ks = vec3 (1.0); //1.6) Brillo especular blanco

	N = normalize (vec3(normal * vec4(inNormal,0.0))); //Las normales del atributo están en c. objeto, hay que pasarlas a c. cámara con la matriz Normal 
	//Primero se convierte inNormal en un vec4 para poder multiplicarlo por la matriz, luego se toman las 3 primeras coordenadas
	//Hay que normalizarlo

	pos = vec3(modelView * vec4(inPos,1.0)); //Pasar coordenadas del punto inPos (c. objeto) a c. cámara

	color = shade(); //El color (variable salida) se calcula en el método shade
	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}

vec3 shade() 
{
	vec3 color = vec3(0.0); //Todos los fragmentos negros
	color = Ia*ka; //Le añade la intensidad de la luz ambiental (pequeña) y el coeficiente

	vec3 L = normalize (Pl - pos); //Extremo (posición luz, la misma que en la cámara) - origen (posición punto), normalizado
	color += Il*kd*dot(N,L); //Al color se le añade la luz difusa = intensidad+cr difusa+cos N^L (prod escalar)

	vec3 V = normalize(-pos); //1.6) Vector de posición del observador (-posición el punto en c camara)
	vec3 R = normalize(reflect (-L,N)); //1.6) Refleja el vector -L sobre el vector N
	float factor = clamp (dot(V,R), 0.0001, 1.0); //1.6) Prod escalar V*R --> Lo ajusta con clamp: dot<min, coge min; dot>max, coge max; dot entre min y max, coge dot
	factor = pow (factor, n); //1.6) (V*R)^n 
	color += Il * ks * factor; //Añade luz especular
	
	return color;
}

//Se ve tenue