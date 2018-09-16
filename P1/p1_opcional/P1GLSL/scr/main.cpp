//PARTE OBLIGATORIA + PARTE OPCIONAL

//Incluye ficheros de cabecera
#include "BOX.h" //Geometr�a que se va a pintar
#include <IGL/IGlib.h> //Librer�a dise�ada para ocultar la parte de OpenGL que se ejecuta en la CPU (cliente)
#include "PIRAMIDE.h" //Incluir geometr�a pir�mide

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp> //Librer�a de vectores
#include <glm/gtc/matrix_transform.hpp>
#include <iostream> //Librer�a para escribir por pantalla

//Idenficadores de los objetos de la escena
int objId = -1;
int cubo2 = -1;
int cubo3 = -1;
int piramide = -1;

//Declaraci�n de CB
//Funciones a las que se va a llamar cada vez que haya un evento
void resizeFunc(int width, int height); //Cambio de tama�o de la ventana (pixeles de ancho y alto)
void idleFunc(); //Funci�n ociosa que se llama cuando el procesador no tiene nada que hacer, anima el cubo
void keyboardFunc(unsigned char key, int x, int y); //Presionar una tecla
void mouseFunc(int button, int state, int x, int y); //Qu� bot�n del rat�n se ha pulsado, si se ha pulsado o liberado, posici�n del rat�n en ese momento

//La matriz view se hace global para poder modificarla en funciones fuera del main
glm::mat4 view (1.0f);
	
int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)
	if (!IGlib::init("../shaders_P1/shader.v9.vert", "../shaders_P1/shader.v9.frag"))
		return -1;
    //^Compila en tiempo de ejecuci�n, enlazando archivos de texto y subi�ndolos a la tarjeta gr�fica (la aplicaci�n compila los shaders)

	//CBs
	//Funciones a las que hay que llamar en cada evento
	IGlib::setResizeCB(resizeFunc);
	IGlib::setIdleCB(idleFunc);
	IGlib::setKeyboardCB(keyboardFunc);
	IGlib::setMouseCB(mouseFunc);

   /*-------------------------------------------------------------------*/	

	//AJUSTAR C�MARA	
	
	//Definir matriz de proyecci�n como la identidad
	glm::mat4 proj = glm::mat4(0.0f);
	
	//2.3) Definir matriz de proy sim�trica con apertura 60�.
	//COLUMNAS, FILAS

	/* | 2n/(r-l)     0      (r+l)/(r-l)       0     |
	   |    0     2n/(t-b)   (t+b)/(t-b)       0     |
	   |    0         0     -(f+n)/(f-n)  -2fn/(f-n) |
	   |    0         0          -1            0     | */
	
	//Sim�trica: top=right=-left=-bottom

	//a00 = 2n/2r = n/r = a11 = 1/tan30
	//a11 = 2n/2t= n/t = n/r = a00 = 1/tan30
	//tan 30� = r/n -> r = n*tan30

	//a20=a21=0 (se anulan los t�rminos)
	
	float near = 0.1f;
	float far = 100.0f;
	float c= 1.0f/(far-near);

	//2.3) Situar la c�mara para que se vea toda la escena, en el punto (0,0,6), mirando a las Z negativas.
	view[3].z = -6.0f; //Desplazamiento en la coordenada z para ver el cubo en su totalidad. 
		
	//Dar valores a la matriz proyecci�n
	proj[0].x = 1.0f / float (tan (3.14159f/6.0f)); //a00
	proj[1].y = proj[0].x; //a11
	proj[2].z = -(far+near)/(far-near);
	proj[2].w = -1.0f;
	proj[3].z = -2.0f*far*near/(far-near); //a32	
	
	//Funciones que definen matriz de proyecci�n y de view	
	//Si no se da valor se cojen valores por defecto
	IGlib::setProjMat(proj); //Se le pasa el valor calculado de la matriz de proyecci�n
	IGlib::setViewMat(view); //Se le pasa el valor calculado de la matriz de vista

	/*-------------------------------------------------------------------*/	
	
	//CREAR PRIMER CUBO

	//createObject: sube info del objeto a la tarjeta gr�fica. Se le pasa el n�mero de tri�ngulos, n� v�rtices,
	//lista tri�ngulos, posiciones de los v�rtices, colores v�rtices, normales, coord texturas, lista tangentes --> En BOX.h
	objId = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex, 
			cubeVertexPos, cubeVertexColor, cubeVertexNormal,cubeVertexTexCoord, cubeVertexTangent);
	//Para crear otro cubo: copiar c�digo y cambiar identificador (inicializar la variable), crear modelview distinta para posicionarlo en otro sitio
			
	glm::mat4 modelMat = glm::mat4(1.0f); //Modelview se inicializa a la identidad
	IGlib::setModelMat(objId, modelMat); //Cambiar matriz modelview de un objeto, copia modelMat en la variable uniform model que est� en shader_allvariables.vert
	
	
	/*-------------------------------------------------------------------*/	
	
	//A�ADIR SEGUNDO CUBO
	
	cubo2 = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex, 
			cubeVertexPos, cubeVertexColor, cubeVertexNormal,cubeVertexTexCoord, cubeVertexTangent);
	
	glm::mat4 modelMat2 = glm::mat4(1.0); //Se crea en la misma posici�n que el cubo 1
	IGlib::setModelMat(cubo2, modelMat2);

	/*-------------------------------------------------------------------*/
	
	//A�ADIR PIR�MIDE
	
	piramide = IGlib::createObj(piramNTriangleIndex, piramNVertex, piramTriangleIndex, 
			piramVertexPos, piramVertexColor, piramVertexNormal,piramVertexTexCoord, piramVertexTangent);
	
	glm::mat4 modelMatPiram = glm::mat4(1.0);
	IGlib::setModelMat(piramide, modelMatPiram);

	/*-------------------------------------------------------------------*/	

	//TEXTURAS

	//Incluir texturas aqu�.
	//7)Asignar una textura al objeto
	IGlib::addColorTex(objId, "..\\img\\color.png");

   /*-------------------------------------------------------------------*/	
	
	//CBs
	IGlib::setIdleCB(idleFunc);
	IGlib::setResizeCB(resizeFunc);
	IGlib::setKeyboardCB(keyboardFunc);
	IGlib::setMouseCB(mouseFunc);
	
	//Mainloop
	IGlib::mainLoop();
	IGlib::destroy();
	return 0;

}


void resizeFunc(int width, int height)
{
	//Matriz de proyecci�n que conserve el Aspect Ratio

	//Inicializar matriz auxiliar 4x4
	glm::mat4 projAR = glm::mat4(0.0f);
	
	//Matriz proyecci�n sim�trica con una apertura de 60�
	/* | 2n/(r-l)     0      (r+l)/(r-l)       0     |
	   |    0     2n/(t-b)   (t+b)/(t-b)       0     |
	   |    0         0     -(f+n)/(f-n)  -2fn/(f-n) |
	   |    0         0          -1            0     | */
	
	//Sim�trica: top=right=-left=-bottom
	//r=n*tan 30
	
	float near = 0.1f;
	float far = 100.0f;	
	float aspectRatio = float (width)/ float (height);
	
	//Definir matriz proyecci�n
	projAR[0].x = 1.0f / float (tan (3.14159f/6.0f)); //a00
	projAR[1].y = (projAR[0].x)*aspectRatio; //a11
	projAR[2].z = -(far+near)/(far-near); //a22
	projAR[2].w = -1.0f; //a23
	projAR[3].z = -2.0f*far*near/(far-near); //a32	
		
	//Funci�n que define la matriz de proyecci�n	
	IGlib::setProjMat(projAR); 

}

//2.6) Utilizar funci�n Idle (procesador en reposo) para rotar el cubo sobre su eje
//Se cambia la matriz model (rota el objeto, no la c�mara (habr�a que cambiar view)).
void idleFunc()
{ 
	//Crear �ngulo de giro que se va a incrementar de una ejecuci�n a otra
	static float angle = 0.0f;	
	angle = (angle < 2.0f * 3.14159f)? angle + 0.0007f : 0.0f; //Si el �ngulo es menor que 2pi, se incrementa. Si no, vuelve a ser 0		
	
	glm::mat4 model (1.0f); //Se inicia una matriz model auxiliar con la identidad

	//Definir matriz de rotaci�n sobre un eje con ese �ngulo --> con funci�n de OpenGL, "rotate"
	model = glm::rotate (model, angle, glm::vec3(1.0f,0.0f,0.0f)); //Se le pasa la matriz a modificar, el �ngulo, y el eje de rotaci�n (x))

	//Se pasa esta matriz a la librer�a
	IGlib::setModelMat(objId, model); 

	/*-------------------------------------------------------------------*/
	
	//MOVER SEGUNDO CUBO

	//Matriz model auxiliar
	glm::mat4 model2 (1.0);
	

	static float angle2 = 0.0f; 
	angle2 = (angle2 < 2.0f * 3.14159f)? angle + 0.0009f : 0.0f; 

	//Cambiar tama�o
	model2 = glm::scale(model2, glm::vec3(0.3f,0.3f,0.3f));
	
	//Orbitar alrededor del primer cubo 
	model2 = glm::rotate (model2, angle, glm::vec3(0.0f,1.0f,0.0f));	
	model2 = glm::translate(model2, glm::vec3(7.0f,0.0f,0.0f));
	
	//Girar sobre su eje y
	model2 = glm::rotate (model2, angle2, glm::vec3(0.0f, 1.0f, 0.0f));
	
	//Pasar la nueva matriz model
	IGlib::setModelMat(cubo2, model2); 

	/*-------------------------------------------------------------------*/
	
	//MOVER PIR�MIDE

	//Matriz model auxiliar
	glm::mat4 modelP (1.0);
	
	//Cambiar tama�o y posici�n	
	modelP = glm::translate(modelP, glm::vec3(0.0f,2.5f,0.0f));
	modelP = glm::rotate(modelP, angle, glm::vec3(0.0f,1.0f,0.0f));
	modelP = glm::scale (modelP, glm::vec3(0.8f,0.8f,0.8f));
	
	IGlib::setModelMat(piramide, modelP); 

}

//Mover la c�mara con el teclado (tipo FPS).
void keyboardFunc(unsigned char key, int x, int y)
{
	std::cout << "Se ha pulsado la tecla " << key << std::endl << std::endl;

	//Matrices de rotaci�n y traslaci�n auxiliares, se inicializan como la matriz identidad
	glm::mat4 rotAux (1.0f);
	glm::mat4 trasAux (1.0f);
	
	switch (key) {
	//Cuando se pulsa la tecla w, la c�mara avanza en su eje z
	case 'w': trasAux = glm::translate(trasAux,glm::vec3(0.0f, 0.0f, 0.5f));
		view = trasAux*view; //Orden correcto para que se mueva en el sistema de coordenadas de la c�mara (local)
		break;
	//Cuando se pulsa la tecla a, la c�mara se mueve hacia la izquierda en su eje x 
	case 'a': trasAux = glm::translate(trasAux,glm::vec3(0.5f, 0.0f, 0.0f));
		view = trasAux*view;
		break;
	//Cuando se pulsa la tecla s, la c�mara retrocede en su eje -z
	case 's': trasAux = glm::translate(trasAux,glm::vec3(0.0f, 0.0f, -0.5f));
		view = trasAux*view;
		break;
	//Cuando se pulsa la tecla d, la c�mara se mueve hacia la derecha en su eje -x 
	case 'd': trasAux = glm::translate(trasAux,glm::vec3(-0.5f, 0.0f, 0.0f));
		view = trasAux*view;
		break;
	//Cuando se pulsa la tecla 4, la c�mara rota sobre su eje y hacia la izquierda (su derecha), en sentido contrario a las agujas del reloj
	case '4': 
		rotAux = glm::rotate(rotAux, 0.1f, glm::vec3(0.0f, -1.0f, 0.0f));
		view = rotAux*view;	//Orden correcto para que se mueva en el sistema de coordenadas de la c�mara (local)
		break;
	//Cuando se pulsa la tecla 6, la c�mara rota sobre su eje y hacia la derecha (su izquierda), en el sentido de las agujas del reloj
	case '6': 
		rotAux = glm::rotate(rotAux, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
		view = rotAux*view;
		break;
	//Cuando se pulsa la tecla 8, la c�mara rota sobre su eje x hacia arriba
	case '8': 
		rotAux = glm::rotate(rotAux, 0.1f, glm::vec3(-1.0f, 0.0f, 0.0f));
		view = rotAux*view;
		break;
	//Cuando se pulsa la tecla 2, la c�mara rota sobre su eje x hacia abajo
	case '2': 
		rotAux = glm::rotate(rotAux, 0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
		view = rotAux*view;
		break;
	}

	//Se le pasa la matriz view transformada a la funci�n
	IGlib::setViewMat(view); 
}

//Rotar la c�mara con el rat�n
void mouseFunc(int button, int state, int x, int y)
{ 
	int xp, xs; //Guardan las coordenadas al pulsar y presionar el bot�n
	
	//Desplazamiento del rat�n
	float despx = xp-xs;
	
	//Matriz de rotaci�n
	glm::mat4 rotx (1.0f);
				
	if (state==0) {
		std::cout << "Se ha pulsado el bot�n ";
		xp=x; //Se guarda la coordenada x del rat�n al pulsar el bot�n			
	}
	else {
		std::cout << "Se ha soltado el bot�n ";
		xs=x; //Se guarda la coordenada x del rat�n al soltar el bot�n
			
		//Si el desplazamiento horizontal es positivo, se mueve la c�mara a la derecha
		if (xp-xs>0) {
			rotx = glm::rotate(rotx, 0.001f*(xp-xs), glm::vec3(0.0f, -1.0f, 0.0f));
		}
		//Si el desplazamiento horizontal es negativo, se mueve la c�mara a la izquierda
		else {
			rotx = glm::rotate(rotx, 0.001f*(xs-xp), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		view = rotx*view;
	}
		
	if (button == 0) std::cout << "de la izquierda del rat�n " << std::endl;
	if (button == 1) std::cout << "central del rat�n " << std::endl;
	if (button == 2) std::cout << "de la derecha del rat�n " << std::endl;

	std::cout << "en la posici�n " << x << " " << y << std::endl << std::endl;
		
	//Se le pasa la matriz view transformada a la funci�n
	IGlib::setViewMat(view);

}

//Marta Fern�ndez de la Mela Salcedo
