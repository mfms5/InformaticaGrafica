//Clara Palomares Calvo
//Marta Fern�ndez de la Mela Salcedo
//PARTE OBLIGATORIA: Luz y c�mara por teclado, aspect ratio, otro cubo.

#include "BOX.h" //Geometr�a
#include "auxiliar.h" //Funciones auxiliares para cargar shaders y texturas

#include <windows.h>

#include <gl/glew.h> //Librer�a GLEW
#include <gl/gl.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> //Dialogar con SO
#include <iostream> //Escribir en consola
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


//////////////////////////////////////////////////////////////
// Datos que se almacenan en la memoria de la CPU
//////////////////////////////////////////////////////////////

//Matrices
glm::mat4	proj = glm::mat4(1.0f);
glm::mat4	view = glm::mat4(1.0f);
glm::mat4	model1 = glm::mat4(1.0f); //Matriz model del primer cubo
glm::mat4   model2 = glm::mat4(1.0f); //PARTE OBLIGATORIA 3: Segunda matriz para el cubo 2
//Solo se tiene una c�mara, definida con proj y view
//Para varios modelos habr� que definir varias model

//////////////////////////////////////////////////////////////
// Variables que nos dan acceso a Objetos OpenGL
//////////////////////////////////////////////////////////////
//3.1) Acceder a shaders y programas (identificadores)
unsigned int vshader; 
unsigned int fshader; 
unsigned int program;


//3.2) Identificadores de las variables uniform y los atributos para poder acceder a ellos
//Variables Uniform 
int uModelViewMat; 
int uModelViewProjMat; 
int uNormalMat; 
//Atributos 
int inPos; 
int inColor; 
int inNormal; 
int inTexCoord;

//Identificador posici�n luz
int posLuz;
// Identificador luz difusa
int Id;

//Vector para la posici�n de la luz
glm::vec3 cLuz(0.0f, 0.0f, 0.0f);
//Vector para la intensidad difusa
glm::vec3 iDiff(1.0f, 1.0f, 1.0f);


//4.1) Variables globales para VAO y VBO
//VAO 
unsigned int vao; 

//VBOs que forman parte del objeto 
unsigned int posVBO; 
unsigned int colorVBO; 
unsigned int normalVBO; 
unsigned int texCoordVBO; 
unsigned int triangleIndexVBO;

//8.2) Identificadores de las variables para configurar texturas, identificadores de la estructura de datos de la textura
unsigned int colorTexId; //Color
unsigned int emiTexId; //Luz emisiva

//8.3) Identificadores de las variables globales del shader que permiten acceder a las variables uniformes
int uColorTex;
int uEmiTex;

//PARTE OBLIGATORIA 4: Evitar que las luces se muevan a la vez que movemos la c�mara
GLint uView = -1; //Sirve para llevar la matriz view al shader de v�rtices


//////////////////////////////////////////////////////////////
// Funciones auxiliares
//////////////////////////////////////////////////////////////
//!!Por implementar

//Declaraci�n de CB
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);

//Funciones de inicializaci�n y destrucci�n
void initContext(int argc, char** argv); //Inicializar contexto
void initOGL(); //Incializar configuraci�n b�sica del cauce
void initShader(const char *vname, const char *fname); //Compilar y linkar shaders
void initObj(); //Crear objeto, subirlo de memoria a la tarjeta gr�fica para crear los VAO y VBO
void destroy(); //Destruir


//Carga el shader indicado, devuele el ID del shader
//!Por implementar
GLuint loadShader(const char *fileName, GLenum type);

//Crea una textura, la configura, la sube a OpenGL, 
//y devuelve el identificador de la textura 
//!!Por implementar
unsigned int loadTex(const char *fileName);


int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)

	initContext(argc, argv);
	initOGL();
	initShader("../shaders_P3/shader.v1.vert", "../shaders_P3/shader.v1.frag");
	initObj();

	//1.6) Bucle de eventos, antes de destroy!!
	glutMainLoop();

	//Cuandos e ssale del bucle, es que el usuario ha cerrado la ventana y hay que liberar recursos
	destroy();

	return 0;
	
}
	
//////////////////////////////////////////
// Funciones auxiliares 

//1.2) Inicializar contexto
void initContext(int argc, char** argv)
{

	glutInit(&argc, argv); //Inicializar contexto por defecto
	glutInitContextVersion(3, 3); //Cambiar al contexto de 3.3
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE); //No permite usar deprecated (no importa en el 3.3)
	glutInitContextProfile(GLUT_CORE_PROFILE); //Versi�n core, acceso a nuevas funcionalidades pero no a las antiguas
	//glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	//1.3) Definir frame buffer y crear ventana --> Espacio de renderizado
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); //Definir frame buffer por defecto
	//^^^^Dos buffers (front y back), colores RGB, buffer de profundidad 
	glutInitWindowSize(500, 500); //Tama�o ventana
	glutInitWindowPosition(0, 0); //Posici�n ventana en pantalla
	glutCreateWindow("Pr�cticas GLSL"); //Crear ventana y contexto

	//1.4)Inicializar extensiones, comprobar si se ha inicializado GLEW correctamente
	glewExperimental = GL_TRUE; 
	GLenum err = glewInit(); //Intenar inicializar glew
	if (GLEW_OK != err) //Comprobart si se ha hecho bien
	{ 
		std::cout << "Error: " << glewGetErrorString(err) << std::endl; //Mostrar error por pantalla si se ha producido alguno
		exit (-1); 
	} 
	const GLubyte *oglVersion = glGetString(GL_VERSION); 
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl; //Mostrar versi�n de OpengGL por pantalla si se ha inicializado bien

	//1.5) Funciones que reciben como par�metro a otra funci�n
	//Se le dice a GLUT a qu� funci�n tiene que llamar cuando se produce cada evento
	glutReshapeFunc(resizeFunc); 
	glutDisplayFunc(renderFunc); 
	glutIdleFunc(idleFunc); 
	glutKeyboardFunc(keyboardFunc); 
	glutMouseFunc(mouseFunc);	

}

void initOGL()
{
	//2.1) 
	glEnable(GL_DEPTH_TEST); //Activar buffer de profundidad
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f); //Poner color de fondo (gris)

	//2.2) Configurar rasterizado y culling
	glFrontFace(GL_CCW); //Interpretar las caras forntales en sentido antihorario
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Decir a la etapa de rasterizado que pinte tri�ngulos rellenando el interior
	glEnable(GL_CULL_FACE); //Activar culling

	//2.3)Definir matrices de vista y proyecci�n perspectiva con �ngulo 60�
	proj = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 50.0f); //Grados, aspect ratio, near, far
	view = glm::mat4(1.0f); 
	view[3].z = -6; //La c�mara est� mirando a las z negativas, colocada en el (0,0,6) (no -6!!)
}


void destroy()
{
	//3.13) LIberar recursos
	glDetachShader(program, vshader); //Separar shader del programa
	glDetachShader(program, fshader); 
	glDeleteShader(vshader); 
	glDeleteShader(fshader); 
	glDeleteProgram(program);

	//4.6) Liberar recursos VBO y VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); //Desactivar VBO popiedades
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Desactivar VBO �ndices
	if (inPos != -1) glDeleteBuffers(1, &posVBO); //Solo se borran si se ha creado 
	if (inColor != -1) glDeleteBuffers(1, &colorVBO); 
	if (inNormal != -1) glDeleteBuffers(1, &normalVBO); 
	if (inTexCoord != -1) glDeleteBuffers(1, &texCoordVBO); 
	glDeleteBuffers(1, &triangleIndexVBO); //Borrar VBO
	
	glBindVertexArray(0); //Desactivar VAO
	glDeleteVertexArrays(1, &vao); //Borrar VAO

	//8.12) Liberar texturas
	glDeleteTextures(1, &colorTexId); 
	glDeleteTextures(1, &emiTexId);

}
void initShader(const char *vname, const char *fname)
{
	//3.6) Crear shader de v�rtices y de fragmentos
	vshader = loadShader(vname, GL_VERTEX_SHADER); 
	fshader = loadShader(fname, GL_FRAGMENT_SHADER);

	//3.7) Crear programa para unir los fragmentos 
	program = glCreateProgram(); 
	glAttachShader(program, vshader); //A�adir s vertices
	glAttachShader(program, fshader); //A�adir s fragmentos
	glLinkProgram(program); //Linkar

	//3.9) Antes de enlazar hay que dar un identificador a los atributos (tambi�n se puede hacer en los shaders)
	glBindAttribLocation(program, 0, "inPos"); 
	glBindAttribLocation(program, 1, "inColor"); 
	glBindAttribLocation(program, 2, "inNormal"); 


	//3.8) Comprobaci�n de errores
	int linked; 
	glGetProgramiv(program, GL_LINK_STATUS, &linked); 
	if (!linked) { 
		//Calculamos una cadena de error 
		GLint logLen; //Obtener cadena de eeror
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen); //Obtener longitud error
		char *logString = new char[logLen]; //Reservar espacio seg�n la longitud obtenida
		glGetProgramInfoLog(program, logLen, NULL, logString); 
		std::cout << "Error: " << logString << std::endl; //Mostrar error por pantalla
		delete logString; //Borrar cadena
		
		glDeleteProgram(program); //Borrar programa
		glDeleteShader(vshader); //Borrar shaders
		glDeleteShader(fshader);
		exit (-1); //Salir
	}

	//3.10) Devuelve los identificadores de uniform, una vez que se han creado los v�rtices correctamente
	uNormalMat = glGetUniformLocation(program, "normal"); //Devuelve identificador (entero positivo o -1 cuando no se usa o no est�n definidas)
	uModelViewMat = glGetUniformLocation(program, "modelView"); 
	uModelViewProjMat = glGetUniformLocation(program, "modelViewProj");

	//8.11) Crear identificadores de acceso de las variables uniform donde van a estar las texturas en el shader
	uColorTex = glGetUniformLocation(program, "colorTex"); 
	uEmiTex = glGetUniformLocation(program, "emiTex");

	//Identificador posici�n luz
	posLuz = glGetUniformLocation(program, "lpos");
	//Identificador intensidad difusa
	Id = glGetUniformLocation(program, "Id");

	//PARTE OBLIGATORIA 4: Variable view para el control de la c�mara con el teclado
	uView = glGetUniformLocation(program, "View");


	//3.11) Devuelve los identificadores de los atributos (se env�a el nombre)
	inPos = glGetAttribLocation(program, "inPos"); //Devuelve 0 
	inColor = glGetAttribLocation(program, "inColor"); //Devuelve 1
	inNormal = glGetAttribLocation(program, "inNormal"); //Devuelve 2
	inTexCoord = glGetAttribLocation(program, "inTexCoord"); //Devuelve 3


}
void initObj()
{
	//4.2)
	glGenVertexArrays(1, &vao); //Crear VAO con su identificador
	glBindVertexArray(vao); //Activar ese VAO. Todas las configuraciones de VBO se har�n sobre el VAO que est� activo
	
	//4.3) No se sube info si no se utiliza, configurar atributos
	if (inPos != -1)
	{
		glGenBuffers(1, &posVBO); //IDenitifcador del buffer donde se van a almacenar las posiciones
		glBindBuffer(GL_ARRAY_BUFFER, posVBO); //Activar el buffer como buffer propiedades, 
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float) * 3, //Al buffer de propiedades que est� activo se le suben las posiciones del cubo
				cubeVertexPos, GL_STATIC_DRAW); //Se le dice el n�mero de bytes (de floats) que ocupa
		glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0); //Configurar,
		//inPos es el identificador del atributo que va a recibir las posiciones
		//VEctor de 3 componentes de tipo float
		//False para NO normalizar los v�rtices
		//Stride y offset para meter varias propiedades en un VBO (no en este caso, est�n a 0)
		glEnableVertexAttribArray(inPos); //Activar el atributo inPos

		//4.5) Inicializar model con la identidad
		//model = glm::mat4(1.0f);
	}

	//Igual pero con el buffer de color
	if (inColor != -1)
	{
		glGenBuffers(1, &colorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float) * 3,
			cubeVertexColor, GL_STATIC_DRAW);
		glVertexAttribPointer(inColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inColor);
	}

	//Igual con normales
	if (inNormal != -1)
	{
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float) * 3,
			cubeVertexNormal, GL_STATIC_DRAW);
		glVertexAttribPointer(inNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inNormal);
	}

	//Igual con texturas, solo que estas se meten en un vector de 2 
	if (inTexCoord != -1)
	{
		glGenBuffers(1, &texCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float) * 2,
			cubeVertexTexCoord, GL_STATIC_DRAW);
		glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTexCoord);
	}

	//4.4) Crear un buffer de �ndices, se configura cada vez que se va a pintar
	glGenBuffers(1, &triangleIndexVBO); //Crear
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO); //Activar
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeNTriangleIndex*sizeof(unsigned int) * 3, 
		cubeTriangleIndex, GL_STATIC_DRAW); //Subir lista de �ndices, cada tri�ngulo son 3 ints
	//Static no se van a cambiar los datos
	//Draw solo se van a subir datos, no se van a leer

	//Se inicializan aqu� las distintas matrices model (a la matriz identidad)
	model1 = glm::mat4(1.0f); //Primer cubo
	model2 = glm::mat4(1.0f); //PARTE OBLIGATORIA 2: matriz model del segundo cubo
	
	//8.10) Crear texturas para color y mapa emisivo
	colorTexId = loadTex("../img/color2.png"); 
	emiTexId = loadTex("../img/emissive.png");
}

//Recibe cadena de caracteres con el nombre, y el tipo de shader
GLuint loadShader(const char *fileName, GLenum type)
{ 
	//3.3) Cargar shader gen�rico
	unsigned int fileLen; 
	char *source = loadStringFromFile(fileName, fileLen); //Devuelve string en la que se almacena el c�digo del shader, devuelve tambi�n la longitud
	
	////////////////////////////////////////////// 
	//Creaci�n y compilaci�n del Shader 
	GLuint shader; 
	shader = glCreateShader(type); //Crear shader pas�ndole el tipo
	glShaderSource(shader, 1, (const GLchar **)&source, (const GLint *)&fileLen); //Asignarle un c�digo al shader
	//^^Todo va en una �nica cadena, se le pasa un puntero a punteros, se le env�a la longitud de la cadena, puede ser un array si hay varias l�neas
	glCompileShader(shader); //Compilar
	delete source; //Borrar el c�digo, no hace falta m�s 
	
	//3.4) C�digo comprobaci�n de errores
	//Comprobamos que se compil� bien 
	GLint compiled; glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled); 
	if (!compiled) { 
		
		//Calculamos una cadena de error 
		GLint logLen; 
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen); //Preguntar longitud de la cadena de error

		std::cout << fileName << std::endl; //Muestra por pantalla el nimbre del fichero (shader que da error)
		char *logString = new char[logLen]; //Reservar memoria
		glGetShaderInfoLog(shader, logLen, NULL, logString); //Poner la cadena de error en el espacio reservado
		std::cout << "Error: " << logString << std::endl; //Mostrarla por pantalla
		delete logString; //Borrar la cadena

		glDeleteShader(shader); //Borrar shader si no se ha compilado
		exit(-1);
	}

	return shader; //3.5) Devuelve el identificador del shader creado
}

//8.4) Cargar y configurar una textura gen�rica
//Recibe como par�metro el nombre de la textura que se va a cargar
unsigned int loadTex(const char *fileName)
{ 
	unsigned char *map; //Crear mipmap
	unsigned int w, h;
	map = loadTexture(fileName, w, h); //Devuelve mipmap donde la textura se ha almacenado

	if (!map) //Si no ha sido capaz de abrir la textura, da error.
	{
		std::cout << "Error cargando el fichero: "
		<< fileName << std::endl;
		exit(-1);
	}

	//8.5) Crear y activar textura
	unsigned int texId; //Identificador de textura
	glGenTextures(1, &texId); //Crear textura
	glBindTexture(GL_TEXTURE_2D, texId); //Activar textura, definir bien su tipo
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)map);
	//^^En una �nica pasada reserva el espacio y sube los datos, se puede cambiar el espacio de la textura despu�s
	//Subir datos a la textura 2D que est� activa ; nivel (0=subir informaci�n al mipmap m�s grande, al nivel 0)
	//RGBA8 = primer formato, hace referencia al formato interno (c�mo se almacenan en la gr�fica), 4 canales de 8 bits ;
	//Tama�o en p�xels de ancho y alto ; 0 por defecto
	//RGBA: el formato de los datos en la memoria es de un espacio con 4 colores, no se le dice qu� tama�o tiene cada canal ;
	//Cada uno de los canales es un unsigned byte

	//8.6) Liberar memoria
	delete[] map; //Borrar el espacio en memoria porque la textura no se va a volver a utilizar

	//8.7) Genera los mipmaps
	//Los niveles de mippmaps depende del tama�o de la textura original
	glGenerateMipmap(GL_TEXTURE_2D);

	//8.8) Configuraci�n modo acceso, 
	//las operaciones que hace la funci�n que se encarga de buscar el valor de las coord textura
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//Mag filter: cuando las coordenadas de los fragmentos continuos no var�an mucho, se selecciona el texel m�s cercano o se interpola entre los 4 pixels m�s cercanos
	//Min filer: las c de los fragmentos est�n alejadas, se interpolan los 4 p�xels m�s pr�ximos
	//Gl_clamp: se queda con el valor que hay en el borde, no repite la textura

	//8.9) Devolver identificador textura
	return texId; 

}

//Funci�n de renderizado
void renderFunc()
{	
	//2.4) Limpiar buffer de color y de profundidad. El resultado es un n�mero con dos bits a 1
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Activar el programa. Todo lo que se pinte despu�s se pintar� con este ptrograma activo
	glUseProgram(program); 

	//5.1) Crear y subir matrices
	glm::mat4 View = view;  //PARTE OBLIGATORIA 4: Asignar a la matriz view el valor del atributo view
	glm::mat4 modelView = view * model1; //Matriz model primer cubo
	glm::mat4 modelViewProj = proj * view * model1;
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));
	
	//Datos en la memoria principal, hay que subirlos a la gr�fica (solo si se necesitan)
	if (uModelViewMat != -1) 
		//Subir una sola matriz 4x4 que se va a asociar con ModelViewMat, false=no hace falta transponer
		//Devuelve el puntero al primer elemento
		glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE, 
			&(modelView[0][0])); 
	if (uModelViewProjMat != -1) 
		glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE, 
			&(modelViewProj[0][0])); 
	if (uNormalMat != -1) 
		glUniformMatrix4fv(uNormalMat, 1, GL_FALSE, 
			&(normal[0][0]));

	//Subir la posici�n de la luz modificada por teclado
	if (posLuz != -1) //Si el identificador no es nulo
		glUniform3fv(posLuz, 1, &(cLuz[0])); 
	//Subir la intensidad difusa modificada por teclado
	if (Id != -1)
		glUniform3fv(Id, 1, &(iDiff[0]));

	//8.13) Si los identificadores son distintos de -1, se asocian las texturas creadas
	//Texturas 
	if (uColorTex != -1) 
	{ 
		glActiveTexture(GL_TEXTURE0); //Activa una unidad de textura, el color va a estar en la unidad 0
		glBindTexture(GL_TEXTURE_2D, colorTexId); //Enlazar textura con la unidad
		glUniform1i(uColorTex, 0); //Se le dice al shader que la textura que va a utilizar est�n en determinada ud de textura activa (0)
	} 
	if (uEmiTex != -1) 
	{ 
		glActiveTexture(GL_TEXTURE0 + 1); //Se mete la luz emisiva en la ud textura 1
		glBindTexture(GL_TEXTURE_2D, emiTexId); 
		glUniform1i(uEmiTex, 1); 
	}
	
	//5.2) Activar VAO y decirle que coja el element array activo y pinte la lista tri�ngulos
	glBindVertexArray(vao); 
	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex*3, 
		GL_UNSIGNED_INT, (void*)0);

	//PARTE OBLIGATORIA 3: NUEVO CUBO
	modelView = view * model2; //Matriz model segundo cubo
	modelViewProj = proj * view * model2; //Matriz modelview*projection
	normal = glm::transpose(glm::inverse(modelView)); //Matriz normal
	
	//Subo nuevamente los datos de la memoria principal a la gr�fica
	if (uModelViewMat != -1)
		glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE,
		&(modelView[0][0]));

	if (uModelViewProjMat != -1)
		glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE,
		&(modelViewProj[0][0]));

	if (uNormalMat != -1)
		glUniformMatrix4fv(uNormalMat, 1, GL_FALSE,
		&(normal[0][0]));

	//Activar VAO y decirle que coja el element array activo y pinte la lista tri�ngulos
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3,
		GL_UNSIGNED_INT, (void*)0);

	//PARTE OBLIGATORIA 4: Asignaci�n de la variable uView, que sirve para que no se muevan las luces cuando movemos la c�mara 
	if (uView != -1)
		glUniformMatrix4fv(uView, 1, GL_FALSE, &(View[0][0]));

	glUseProgram(NULL);

	//1.7) Intercambiar buffers a terminar de pintar, se pone al final
	glutSwapBuffers();


}

//6) Ajustar viewport al cambiar el tama�o de la ventana
void resizeFunc(int width, int height)
{
	glViewport(0, 0, width, height); //Funci�n que cambia el viewport
	//^^ Coordenadas proyectadas a coordenadas de pantalla (ajustar escena al tama�o de ventana).
	
	//Aspect ratio: Crear matriz de proyecci�n perspectiva que conserve las proporciones, apertura=60�, near=0.1, far=100
	float ar = float(width)/float(height);
	proj = glm::perspective(glm::radians(60.0f), ar, 0.1f, 100.0f);

	glutPostRedisplay(); //Genera un evento de renderizado (en lugar de llamar a renderfunc)
	
}

void idleFunc()
{
	//7.1) Modificar matriz model 
	model1 = glm::mat4(1.0f);
	static float angle = 0.0f;
	angle = (angle > 3.141592f *2.0f) ? 0 : angle + 0.0003f;

	model1 = glm::rotate(model1, angle, glm::vec3(0.0f, 1.0f, 0.0f));

	//PARTE OBLIGATORIA 3: Rotaciones del segundo cubo
	model2 = glm::mat4(1.0f);
	model2 = glm::scale (model2, glm::vec3(0.3f, 0.3f, 0.3f));
	model2 = glm::rotate(model2, -angle, glm::vec3(0.0f, 1.0, 0.0f)); //Rotaci�n sobre su propio eje Y
	model2 = glm::translate(model2, glm::vec3(7.0f, 0.0f, 0.0f));
	//model2 = Matriz Rotaci�n (�rbita) * Matriz Traslaci�n * Matriz Escalado 
	model2 = glm::rotate(model2, -angle, glm::vec3(0.0f, 1.0f, 0.0f));
	
	//7.2) Lanzar evento de renderizado
	glutPostRedisplay();

}

void keyboardFunc(unsigned char key, int x, int y)
{
	std::cout << "Se ha pulsado la tecla " << key << std::endl << std::endl;
		
	glm::mat4 moveView(1.0f);  //Matriz auxiliar para mover la c�mara (traslaciones y rotaciones)

	switch (key) {	
	// Subir
	case 'i':
		cLuz.y = cLuz.y+0.3f;
		break;
	//Bajar
	case 'k' :
		cLuz.y = cLuz.y-0.3f;
		break;
	//Izquierda
	case 'j' : 
		cLuz.x = cLuz.x-0.3f;
		break;
	//Derecha
	case 'l' : 
		cLuz.x = cLuz.x+0.3f;
		break;
	//Atr�s
	case 'u': 
		cLuz.z = cLuz.z-0.3f;
		break;
	//Adelante
	case 'o': 
		cLuz.z = cLuz.z+0.3f;
		break;
	//M�s intensidad
	case '+':
		iDiff += (0.3f);
		break;
	case '-':
		iDiff -= (0.3f);
		break;
	//C�mara
	case 'w': //Movimiento hacia adelante (eje Z)
		moveView = glm::translate(moveView, glm::vec3(0.0f, 0.0f, 0.1f));
		view = moveView * view;
		break;
	case 's': //Movimiento hacia atr�s (eje Z)
		moveView = glm::translate(moveView, glm::vec3(0.0f, 0.0f, -0.1f));
		view = moveView * view;
		break;
	case 'a': //'Pasos' a la derecha (eje X)
		moveView = glm::translate(moveView, glm::vec3(0.1f, 0.0f, 0.0f));
		view = moveView * view;
		break;
	case 'd': //'Pasos' a la izquierda (eje X)
		moveView = glm::translate(moveView, glm::vec3(-0.1f, 0.0f, 0.0f));
		view = moveView * view;
		break;
	case '6': //Rotaci�n a la derecha sobre el eje Y (mirar a la derecha)
		moveView = glm::rotate(moveView, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
		view = moveView * view;
		break;
	case '4': //Rotaci�n a la izquierda sobre el eje Y (mirar a la izquierda)
		moveView = glm::rotate(moveView, -0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
		view = moveView * view;
		break;
	case '8': //Rotaci�n hacia arriba sobre el eje X (mirar hacia arriba)
		moveView = glm::rotate(moveView, -0.01f, glm::vec3(1.0f, 0.0f, 0.0f));
		view = moveView * view;
		break;
	case '2': //Rotaci�n hacia abajo sobre el eje X (mirar hacia abajo)
		moveView = glm::rotate(moveView, 0.01f, glm::vec3(1.0f, 0.0f, 0.0f));
		view = moveView * view;
		break;
	}
	
}

//Rotar la c�mara con el rat�n
void mouseFunc(int button, int state, int x, int y)
{}









