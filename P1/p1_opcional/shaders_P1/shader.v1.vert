#version 330 core

//Atributo inPos: posición del vértice con el que queremos trabajar
//Todas las uniform y atributos están en shader_allvariables
in vec3 inPos;		

void main()
{

	//Definir matrices identidad para proyectar
	mat4 view = mat4(1.0);
	mat4 proj = mat4(0.0);
	
	//1.5) Definir matriz de proy simétrica con apertura 60º.

	
	/* | 2n/(r-l)     0      (r+l)/(r-l)       0     |
	   |    0     2n/(t-b)   (t+b)/(t-b)       0     |
	   |    0         0     -(f+n)/(f-n)  -2fn/(f-n) |
	   |    0         0          -1            0     | */

	//COLUMNAS,FILAS
	//Simétrica: top=right=-left=-bottom

	//a00 = 2n/2r = n/r = a11 = 1/tan30
	//a11 = 2n/2t= n/t = n/r = a00 = 1/tan30
	//tan 30º = r/n -> r = n*tan30

	//a20=a21=0 (se anulan los términos)
	
	float near = 0.1;
	float far = 100.0;
	float c= 1.0/(far-near);

	//1.5)Situar la cámara para que se vea toda la escena
	view[3].z = -3.0; //Desplazamiento en la coordenada z para ver el cubo en su totalidad. 
	//x=1((distancia vértice))/tan30 = 1.73 + distancia al centro = 2.73 -> 3 [ver pdf]

	proj[0].x = 1.0 / tan (3.14159/6.0); //a00
	proj[1].y = proj[0].x; //a11
	proj[2].zw = vec2 (-(far+near)*c, -1.0); //a22 y a23
	proj[3].z = -2.0*far*near*c; //a32

	//Dar valor a la posición
	//Hay que convertir inPos en un vector de 4 para poderlo multiplicar. Se pone 1.0 porque es un punto
	gl_Position = proj*view*vec4 (inPos,1.0);

	
}

//Esto es una chapuza (los valores están fijos, siempre se tienen las mismas matrices de proyección, cámara etc) --> Usar variables uniform