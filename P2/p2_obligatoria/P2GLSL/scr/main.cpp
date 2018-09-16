#include "BOX.h"
#include <IGL/IGlib.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


//Idenficadores de los objetos de la escena
int objId =-1;

//Declaraci�n de CB
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);

glm::mat4 view = glm::mat4(1.0);
glm::mat4 modelMat = glm::mat4(1.0f);

int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)
	if (!IGlib::init("../shaders_P2/shader.v3.vert", "../shaders_P2/shader.v7.frag"))
		return -1;
   
	//CBs
	IGlib::setResizeCB(resizeFunc);
	IGlib::setIdleCB(idleFunc);
	IGlib::setKeyboardCB(keyboardFunc);
	IGlib::setMouseCB(mouseFunc);

	//Se ajusta la c�mara
	//Si no se da valor se cogen valores por defecto
	
	view[3].z = -6;
	
	glm::mat4 proj = glm::mat4(1.0);
	float f = 1.0f / tan(3.141592 / 6);
	float far = 10.0;
	float near = 0.1;

	proj[0].x = f;
	proj[1].y = f;
	proj[2].z = (far + near) / (near - far);
	proj[2].w = -1.0f;
	proj[3].z = (2.0f * far * near) / (near - far);
	proj[3].w = 0.0f;
	IGlib::setProjMat(proj);
	IGlib::setViewMat(view);

	//Creamos el objeto que vamos a visualizar
	objId = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex, 
			cubeVertexPos, cubeVertexColor, cubeVertexNormal,cubeVertexTexCoord, cubeVertexTangent);
	IGlib::addColorTex(objId, "../img/color2.png");
	IGlib::addEmissiveTex(objId, "../img/emissive.png");
	IGlib::addSpecularTex(objId, "../img/specMap.png");
		
	IGlib::setModelMat(objId, modelMat);
	
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
	//Ajusta el aspect ratio al tama�o de la venta
}

void idleFunc()
{
	glm::mat4 modelMat(1.0f);
	static float angle = 0.0f;
	angle = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.0003f;

	modelMat = glm::rotate(modelMat, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	//modelMat = glm::translate(modelMat, glm::vec3(3.0f, 0.0f, 0.0f));

	IGlib::setModelMat(objId, modelMat);
}

void keyboardFunc(unsigned char key, int x, int y)
{
	std::cout << "Se ha pulsado la tecla " << key << std::endl << std::endl;

	glm::mat4 trasAux = glm::mat4(1.0f);
	glm::mat4 rotAux = glm::mat4(1.0f);
	glm::mat4 modelAux = glm::mat4(1.0f);
		
	switch (key) {
	case 'w': trasAux = glm::translate(trasAux, glm::vec3(0.0f, 0.0f, 0.5f));
		view = trasAux*view;
		break;
	case 's': trasAux = glm::translate(trasAux, glm::vec3(0.0f, 0.0f, -0.5f));
		view = trasAux*view;
		break;
	case 'a': trasAux = glm::translate(trasAux, glm::vec3(0.5f, 0.0f, 0.0f));
		view = trasAux*view;
		break;
	case 'd': trasAux = glm::translate(trasAux, glm::vec3(-0.5f, 0.0f, 0.0f));
		view = trasAux*view;
		break;
	case 'q': rotAux = glm::rotate(rotAux, 0.1f, glm::vec3(0.0f, -1.0f, 0.0f));
		view = rotAux*view;
		break;
	case 'e': rotAux = glm::rotate(rotAux, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
		view = rotAux*view;
		break;
	case 'm': modelAux = glm::translate(modelAux, glm::vec3(0.0f, 0.0f, -0.1f));
		modelMat = modelAux*modelMat;
		break;
	}

	IGlib::setViewMat(view);
	IGlib::setModelMat(objId, modelMat);

}

void mouseFunc(int button, int state, int x, int y)
{
	if (state==0)
		std::cout << "Se ha pulsado el bot�n ";
	else
		std::cout << "Se ha soltado el bot�n ";
	
	if (button == 0) std::cout << "de la izquierda del rat�n " << std::endl;
	if (button == 1) std::cout << "central del rat�n " << std::endl;
	if (button == 2) std::cout << "de la derecha del rat�n " << std::endl;

	std::cout << "en la posici�n " << x << " " << y << std::endl << std::endl;
}
