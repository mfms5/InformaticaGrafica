#include "BOX.h"
#include "auxiliar.h"
#include "PLANE.h"

#include <windows.h>

#include <gl/glew.h>
#include <gl/gl.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 
#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cstdlib>

#define RAND_SEED 31415926
#define SCREEN_SIZE 500,500

//////////////////////////////////////////////////////////////
// Datos que se almacenan en la memoria de la CPU
//////////////////////////////////////////////////////////////

//Matrices
glm::mat4	proj = glm::mat4(1.0f);
glm::mat4	view = glm::mat4(1.0f);
glm::mat4	model = glm::mat4(1.0f);


//////////////////////////////////////////////////////////////
// Variables que nos dan acceso a Objetos OpenGL
//////////////////////////////////////////////////////////////
float angle = 0.0f;

//VAO
// Guarda la configuración de los VBO.
unsigned int vao;
//2.7) VAO para el plano
unsigned int planeVAO; 

//VBOs que forman parte del objeto
unsigned int posVBO;
unsigned int colorVBO;
unsigned int normalVBO;
unsigned int texCoordVBO;
unsigned int triangleIndexVBO;
// 2.7) VBO para el plano
unsigned int planeVertexVBO;

unsigned int colorTexId;
unsigned int emiTexId;

//Shaders y programa para enlazarlos
unsigned int vshader;
unsigned int fshader;
unsigned int program;

//Variables Uniform 
// Identificadores de las variables uniform pasa subir información al shader
int uModelViewMat;
int uModelViewProjMat;
int uNormalMat;

//Texturas Uniform
int uColorTex;
int uEmiTex;

//Atributos
int inPos;
int inColor;
int inNormal;
int inTexCoord;

//2.3) Variables de acceso a los shaders postProcessing
unsigned int postProccesVShader; 
unsigned int postProccesFShader; 
unsigned int postProccesProgram; 
//Uniform 
unsigned int uColorTexPP; 
//Atributos 
int inPosPP;

//2.14) Identificadores del FBO y las texturas
unsigned int fbo; 
unsigned int colorBuffTexId; 
unsigned int depthBuffTexId;


//////////////////////////////////////////////////////////////
// Funciones auxiliares
//////////////////////////////////////////////////////////////

//Declaración de CB
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);

// Función que permite renderizar muchos cubos
void renderCube();

//Funciones de inicialización y destrucción
void initContext(int argc, char** argv);
void initOGL();
void initShaderFw(const char *vname, const char *fname); //Inicializar shaders
void initObj();
void destroy();
//2.4)
void initShaderPP(const char *vname, const char *fname);
//2.7) Subir info del plano a la gráfica
void initPlane();
//2.15) Función que inicializa el FBO
void initFBO();
//2.16) Cambiar tamaño FBO
void resizeFBO(unsigned int w, unsigned int h);


//Carga el shader indicado, devuele el ID del shader
//!Por implementar
GLuint loadShader(const char *fileName, GLenum type);

//Crea una textura, la configura, la sube a OpenGL, 
//y devuelve el identificador de la textura 
//!!Por implementar
unsigned int loadTex(const char *fileName);

//////////////////////////////////////////////////////////////
// Nuevas variables auxiliares
//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
// Nuevas funciones auxiliares
//////////////////////////////////////////////////////////////
//!!Por implementar


int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)

	initContext(argc, argv);
	initOGL();
	initShaderFw("../shaders_P4/fwRendering.v0.vert", "../shaders_P4/fwRendering.v0.frag"); // Igual que initShader de la práctica anterior
	initObj();
	//2.6) Inicializar shaders del PP
	initShaderPP( "../shaders_P4/postProcessing.v0.vert", "../shaders_P4/postProcessing.v0.frag");
	//2.10) Se inicializa el plano después del sahder porque tiene que saber el valor de inPos
	initPlane();

	//2.18)	Inicializar funciones de FBO y resizeFBO
	initFBO(); 
	resizeFBO(SCREEN_SIZE); //Screen_size es una constante definida con el tamaño de la ventana

	glutMainLoop();

	destroy();

	return 0;
}

//////////////////////////////////////////
// Funciones auxiliares 
void initContext(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	//glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_SIZE);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Prácticas GLSL");

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}

	const GLubyte *oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;

	glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
}

void initOGL()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	proj = glm::perspective(glm::radians(60.0f), 1.0f, 1.0f, 50.0f);
	view = glm::mat4(1.0f);
	view[3].z = -25.0f;
}


void destroy()
{
	glDetachShader(program, vshader);
	glDetachShader(program, fshader);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glDeleteProgram(program);

	//2.5) Liberar shaders del PP
	glDetachShader(postProccesProgram, postProccesVShader); 
	glDetachShader(postProccesProgram, postProccesFShader); 
	glDeleteShader(postProccesVShader);
	glDeleteShader(postProccesFShader); 
	glDeleteProgram(postProccesProgram);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (inPos != -1) glDeleteBuffers(1, &posVBO);
	if (inColor != -1) glDeleteBuffers(1, &colorVBO);
	if (inNormal != -1) glDeleteBuffers(1, &normalVBO);
	if (inTexCoord != -1) glDeleteBuffers(1, &texCoordVBO);
	glDeleteBuffers(1, &triangleIndexVBO);

	//2.9) Borrar VAO y VBO del plano
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glDeleteBuffers(1, &planeVertexVBO); 
	glBindVertexArray(0); 
	glDeleteVertexArrays(1, &planeVAO);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &colorTexId);
	glDeleteTextures(1, &emiTexId);

	//2.17) Liberar FBO (primero se activa, después se borra) y sus texturas
	glBindFramebuffer(GL_FRAMEBUFFER,0); 
	glDeleteFramebuffers(1, &fbo); 
	glDeleteTextures(1, &colorBuffTexId);
	glDeleteTextures(1, &depthBuffTexId);
}

void initShaderFw(const char *vname, const char *fname)
{
	vshader = loadShader(vname, GL_VERTEX_SHADER);
	fshader = loadShader(fname, GL_FRAGMENT_SHADER);

	program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);

	glBindAttribLocation(program, 0, "inPos");
	glBindAttribLocation(program, 1, "inColor");
	glBindAttribLocation(program, 2, "inNormal");
	glBindAttribLocation(program, 3, "inTexCoord");


	glLinkProgram(program);

	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

		char *logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete logString;

		glDeleteProgram(program);
		program = 0;
		exit(-1);
	}

	uNormalMat = glGetUniformLocation(program, "normal");
	uModelViewMat = glGetUniformLocation(program, "modelView");
	uModelViewProjMat = glGetUniformLocation(program, "modelViewProj");

	uColorTex = glGetUniformLocation(program, "colorTex");
	uEmiTex = glGetUniformLocation(program, "emiTex");

	inPos = glGetAttribLocation(program, "inPos");
	inColor = glGetAttribLocation(program, "inColor");
	inNormal = glGetAttribLocation(program, "inNormal");
	inTexCoord = glGetAttribLocation(program, "inTexCoord");
}

void initObj()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if (inPos != -1)
	{
		glGenBuffers(1, &posVBO);
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float) * 3,
			cubeVertexPos, GL_STATIC_DRAW);
		glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inPos);
	}

	if (inColor != -1)
	{
		glGenBuffers(1, &colorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float) * 3,
			cubeVertexColor, GL_STATIC_DRAW);
		glVertexAttribPointer(inColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inColor);
	}

	if (inNormal != -1)
	{
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float) * 3,
			cubeVertexNormal, GL_STATIC_DRAW);
		glVertexAttribPointer(inNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inNormal);
	}


	if (inTexCoord != -1)
	{
		glGenBuffers(1, &texCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float) * 2,
			cubeVertexTexCoord, GL_STATIC_DRAW);
		glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTexCoord);
	}

	glGenBuffers(1, &triangleIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		cubeNTriangleIndex*sizeof(unsigned int) * 3, cubeTriangleIndex,
		GL_STATIC_DRAW);

	model = glm::mat4(1.0f);

	colorTexId = loadTex("../img/color2.png");
	emiTexId = loadTex("../img/emissive.png");
}

GLuint loadShader(const char *fileName, GLenum type)
{
	unsigned int fileLen;
	char *source = loadStringFromFile(fileName, fileLen);

	//////////////////////////////////////////////
	//Creación y compilación del Shader
	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1,
		(const GLchar **)&source, (const GLint *)&fileLen);
	glCompileShader(shader);
	delete source;

	//Comprobamos que se compilo bien
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

		char *logString = new char[logLen];
		glGetShaderInfoLog(shader, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete logString;

		glDeleteShader(shader);
		exit(-1);
	}

	return shader;
}

unsigned int loadTex(const char *fileName)
{
	unsigned char *map;
	unsigned int w, h;
	map = loadTexture(fileName, w, h);

	if (!map)
	{
		std::cout << "Error cargando el fichero: "
			<< fileName << std::endl;
		exit(-1);
	}

	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, (GLvoid*)map);
	delete[] map;
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	return texId;
}


void renderFunc()
{
	//2.21) Activar FBO antes de renderizar y de limpiar
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//Limpiar color y profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/**/
	glUseProgram(program);

	//Texturas
	if (uColorTex != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexId);
		glUniform1i(uColorTex, 0);
	}

	if (uEmiTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, emiTexId);
		glUniform1i(uEmiTex, 1);
	}


	model = glm::mat4(2.0f);
	model[3].w = 1.0f;
	model = glm::rotate(model, angle, glm::vec3(1.0f, 1.0f, 0.0f));
	renderCube(); //Renderizar el primer cubo

	//Genera siempre los mismos valores aleatorios para que los cubos estén en la misma posición de un renderizado a otra
	std::srand(RAND_SEED);
	for (unsigned int i = 0; i < 10; i++)
	{
		float size = float(std::rand() % 3 + 1);

		glm::vec3 axis(glm::vec3(float(std::rand() % 2),
			float(std::rand() % 2), float(std::rand() % 2)));
		if (glm::all(glm::equal(axis, glm::vec3(0.0f))))
			axis = glm::vec3(1.0f);

		float trans = float(std::rand() % 7 + 3) * 1.00f + 0.5f;
		glm::vec3 transVec = axis * trans;
		transVec.x *= (std::rand() % 2) ? 1.0f : -1.0f;
		transVec.y *= (std::rand() % 2) ? 1.0f : -1.0f;
		transVec.z *= (std::rand() % 2) ? 1.0f : -1.0f;

		//Cambia la posición, rotación, tamaño etc de los cubos
		model = glm::rotate(glm::mat4(1.0f), angle*2.0f*size, axis);
		model = glm::translate(model, transVec);
		model = glm::rotate(model, angle*2.0f*size, axis);
		model = glm::scale(model, glm::vec3(1.0f / (size*0.7f)));
		renderCube();
		
	}
	//*/

	//2.21) Activar FB por defecto
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//2.12) Comentar el código que pintaba los cubos. Añadir código para pintar el plano
	// Se descomenta en el 2.20 

	//Desactivar culling y profundidad porque se va a pintar un plano frente a la cámara y
	//no va a haber ningún fragmento que oculte a otro, no hacen falta
	glDisable(GL_CULL_FACE); 
	glDisable(GL_DEPTH_TEST); 

	//3.1) Activa blending, antes del pintado del plano y después del pintado de la escena en el FBO
	glEnable(GL_BLEND); 
	//El valor por el que hay que multiplicar el fragmento es alpha
	//El valor por el que hay que multiplicar el frame buffer por defecto es -alpha
	//3.2) Cambiar esta línea para mejorar el algoritmo
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_CONSTANT_ALPHA, GL_CONSTANT_COLOR); //Se ha cambiado el orden color-alpha
	glBlendColor(0.8, 0.8f, 0.8f, 0.3f); //Variar los valores según se quiera dar más peso a cada imagen
	//Se le indica que la operación que se va a realizar es la suma
	glBlendEquation(GL_FUNC_ADD); 

	//Activar postprocesado
	//2.22) Subir la textura al shader de post-proceso
	glUseProgram(postProccesProgram); 

	if (uColorTexPP != -1) 
	{ 
		glActiveTexture(GL_TEXTURE0); 
		glBindTexture(GL_TEXTURE_2D, colorBuffTexId); 
		glUniform1i(uColorTexPP, 0); 
	}
		
	//Activar VBO con el plano
	glBindVertexArray(planeVAO); 
	// Decir que los fragmentos se pinten de forma no indexada, sino como strip 
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
		
	//Volver a activar culling y profundidad
	glEnable(GL_CULL_FACE); 
	glEnable(GL_DEPTH_TEST); 
	//3.1) Desactivar blending después de pintar y fusionar
	glDisable(GL_BLEND);

		
	//Desactivar el uso del programa
	glUseProgram(NULL);

	glutSwapBuffers();
}

void renderCube()
{
	//Como la matriz model cambia, crea una para cada cubo
	glm::mat4 modelView = view * model;
	glm::mat4 modelViewProj = proj * view * model;
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));

	if (uModelViewMat != -1)
		glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE,
		&(modelView[0][0]));
	if (uModelViewProjMat != -1)
		glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE,
		&(modelViewProj[0][0]));
	if (uNormalMat != -1)
		glUniformMatrix4fv(uNormalMat, 1, GL_FALSE,
		&(normal[0][0]));
	
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3,
		GL_UNSIGNED_INT, (void*)0);
}



void resizeFunc(int width, int height)
{
	glViewport(0, 0, width, height);
	proj = glm::perspective(glm::radians(60.0f), float(width) /float(height), 1.0f, 50.0f);

	//2.19) Llamar a la función que cambia el tamaño del FBO cuando cambia el tamaño de la pantallo
	resizeFBO(width, height);

	glutPostRedisplay();
}

void idleFunc()
{
	angle = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.02f;
	
	glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y){}
void mouseFunc(int button, int state, int x, int y){}

//2.4) Función para inicializar los shader posProcessing
void initShaderPP(const char *vname, const char *fname)
{
	//Cargar shaders
	postProccesVShader = loadShader(vname, GL_VERTEX_SHADER); 
	postProccesFShader = loadShader(fname, GL_FRAGMENT_SHADER); 
	
	//Crear programa
	postProccesProgram = glCreateProgram(); 
	//Enlazar shaders
	glAttachShader(postProccesProgram, postProccesVShader);
	glAttachShader(postProccesProgram, postProccesFShader); 
	
	//Activar programa y darle el identificador 0
	glBindAttribLocation(postProccesProgram, 0, "inPos"); 
	
	//Hacer linkado
	glLinkProgram(postProccesProgram); 
	
	int linked; 
	glGetProgramiv(postProccesProgram, GL_LINK_STATUS, &linked); 	
	//Comprobar si hay errores de linkado
	if (!linked) { 
		//Calculamos una cadena de error 
		GLint logLen; 
		glGetProgramiv(postProccesProgram, GL_INFO_LOG_LENGTH, &logLen);

		char *logString = new char[logLen]; 
		glGetProgramInfoLog(postProccesProgram, logLen, NULL, logString); 
		std::cout << "Error en el enlazado del pp. " << logString << std::endl; 
		std::cout << "Error: " << logString << std::endl; 
		delete logString; 
		
		glDeleteProgram(postProccesProgram); 
		postProccesProgram = 0;
		exit(-1); 
	} 
	
	uColorTexPP = glGetUniformLocation(postProccesProgram, "colorTex"); 
	inPosPP = glGetAttribLocation(postProccesProgram, "inPos");
}

//2.7) Subir info del plano a la gráfica
void initPlane()
{
	//Generar y activar VAO
	glGenVertexArrays(1, &planeVAO); 
	glBindVertexArray(planeVAO); 

	//Generar VBO
	glGenBuffers(1, &planeVertexVBO); 
	//Activar VBO
	glBindBuffer(GL_ARRAY_BUFFER, planeVertexVBO); 
	//Reservar espacio para VBO, 4 vértices*float*3componentes
	glBufferData(GL_ARRAY_BUFFER, planeNVertex*sizeof(float) * 3, 
		planeVertexPos, GL_STATIC_DRAW); 
	
	glVertexAttribPointer(inPosPP, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(inPosPP);

}

//2.15) Función que inicializa el FBO
void initFBO()
{
	//Pedir identificador para FBO y las dos texturas
	glGenFramebuffers(1, &fbo); 
	glGenTextures(1, &colorBuffTexId); 
	glGenTextures(1, &depthBuffTexId);

}

//2.16) Función que cambia el tamaño de las texturas que tiene el FBO asocidadas en función del tamaño de la ventana
void resizeFBO(unsigned int w, unsigned int h) 
{
	//Reservar espacio de color necesario
	//RGBA de 32 bits por color
	//No se pueden utilizar midmaps porque se está escribiendo sobre un único nivel de la textura
	glBindTexture(GL_TEXTURE_2D, colorBuffTexId); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0,
		GL_RGBA, GL_FLOAT, NULL); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	//Reservar memoria para el buffer de profundidad
	//El formato ahora no es de color, sino de profundidad que se almacena en 24bits
	glBindTexture(GL_TEXTURE_2D, depthBuffTexId); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0, 
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Asociar texturas al FBO
	//Activar buffer
	glBindFramebuffer(GL_FRAMEBUFFER, fbo); 
	//Meter textura de color, da igual el número del attachment, es una textura 2D, se le da el identificador de la textura, 0=level del midmap
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
		GL_TEXTURE_2D, colorBuffTexId, 0); 
	//Igual pero con la textura de profundidad
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 
		depthBuffTexId, 0);

	//Crear un array con los color attachment
	//Al color attachment 0 (el que se utiliza) le corresponde la salida 0
	//El 1 es la cantidad de salidas que quieres enlazar
	const GLenum buffs[1] = {GL_COLOR_ATTACHMENT0}; 
	//A la salida 0 le corresponde el color attachment 0
	glDrawBuffers(1, buffs); 

	//Comprobar que el FBO se ha creado adecuadamente	
	if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER)) 
	{ 
		std::cerr << "Error configurando el FBO" << std::endl; 
		exit(-1); 
	} 

	//Llamar a la función que activa un framebuffer, el 0 (FB por defecto)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}