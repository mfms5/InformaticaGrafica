#ifndef __PIRAMIDEFILE__
#define __PIRAMIDEFILE__

//Describimos una pir�mide de base cuadrada

//Se replican v�rtices para que cada cara 
//tenga una normal distinta.

//N�mero de v�rtices y n�mero de �ndices
const int piramNVertex = 16; // 4 (base) + 4*3 (4 caras de 3 v�rtices casda una)
const int piramNTriangleIndex = 6; // 4 caras de un tri�ngulo + 2 tri�ngulos en la base

//V�rtices que componen los tri�ngulos de cada cara
const unsigned int piramTriangleIndex[] = {
	//Cara z = 1
	0,1,2,			
	//Cara z = -1
	3,4,5,			
	//Cara x = 1
	6,7,8,			
	//Cara x = -1
	9,10,11,
	//Cara y = -1 (base)
	12,14,13,		12,15,14
};

//Posici�n de los vertices
const float piramVertexPos[] = { 
	//Cara z = 1
	0.0f,  1.0f,  0.0f, //0
   -1.0f, -1.0f,  1.0f, //1
    1.0f, -1.0f,  1.0f, //2
	 
	//Cara z = -1		   
	0.0f,  1.0f,  0.0f, //3
    1.0f, -1.0f, -1.0f, //4
   -1.0f, -1.0f, -1.0f, //5
	 
	//Cara x = 1		   
	0.0f,  1.0f,  0.0f, //6
    1.0f, -1.0f,  1.0f, //7
    1.0f, -1.0f, -1.0f, //8
	
	//Cara x = -1		   
	0.0f,  1.0f,  0.0f, //9
   -1.0f, -1.0f, -1.0f, //10
   -1.0f, -1.0f,  1.0f, //11
	
	//Cara y = -1 (base)	   
	-1.0f,	-1.0f,	 1.0f, //12
	 1.0f,	-1.0f,	 1.0f, //13
	 1.0f,	-1.0f,	-1.0f, //14
	-1.0f,	-1.0f,	-1.0f, //15
 };

//Normales
const float piramVertexNormal[] = { 
	//Cara z = 1
	0.0f,	0.0f,	 1.0f, 
	0.0f,	0.0f,	 1.0f, 
	0.0f,	0.0f,	 1.0f, 
	
	//Cara z = -1		   
	0.0f,	0.0f,	-1.0f, 
	0.0f,	0.0f,	-1.0f, 
	0.0f,	0.0f,	-1.0f, 
	
	//Cara x = 1		   
	1.0f,	0.0f,	 0.0f, 
	1.0f,	0.0f,	 0.0f, 
	1.0f,	0.0f,	 0.0f, 
	
	//Cara x = -1		   
	-1.0f,	0.0f,	 0.0f, 
	-1.0f,	0.0f,	 0.0f, 
	-1.0f,	0.0f,	 0.0f, 
	
	//Cara y = -1		   
	0.0f,	-1.0f,	0.0f, 
	0.0f,	-1.0f,	0.0f, 
	0.0f,	-1.0f,	0.0f, 
	0.0f,	-1.0f,	0.0f,  
 };


//Color de los vertices
//A todos los v�rtices de la cara se les da el mismo color
const float piramVertexColor[] = { 
	//Cara z = 1
	0.0f,	1.0f,	 1.0f, 
	0.0f,	1.0f,	 1.0f, 
	0.0f,	1.0f,	 1.0f, 
	
	//Cara z = -1		   
	0.0f,	0.6f,	 1.0f, 
	0.0f,	0.6f,	 1.0f, 
	0.0f,	0.6f,	 1.0f, 
	
	//Cara x = 1		   
	1.0f,	0.8f,	 0.0f, 
	1.0f,	0.8f,	 0.0f, 
	1.0f,	0.8f,	 0.0f, 
	
	//Cara x = -1		   
	1.0f,	0.2f,	 0.6f, 
	1.0f,	0.2f,	 0.6f, 
	1.0f,	0.2f,	 0.6f, 
	
	//Cara y = -1		   
	0.8f,	0.3f,	1.0f, 
	0.8f,	0.3f,	1.0f, 
	0.8f,	0.3f,	1.0f, 
	0.8f,	0.3f,	1.0f  
 };

//Cood. de textura de los vertices
//Una por v�rtice y cara
const float piramVertexTexCoord[] = { 
	//Cara z = 1
	0.5f, 1.0f, //0
	0.0f, 0.0f, //1
	1.0f, 0.0f, //2	 

	//Cara z = -1
	0.5f, 1.0f, //3
	0.0f, 0.0f, //4
	1.0f, 0.0f, //5
	
	//Cara x = 1	
	0.5f, 1.0f, //6
	0.0f, 0.0f, //8
	1.0f, 0.0f, //7

	
	//Cara x = -1
	0.5f, 1.0f, //9
	0.0f, 0.0f, //10
	1.0f, 0.0f, //11
	
	//Cara y = -1
	0.0f, 1.0f, //12
	1.0f, 1.0f, //13
	1.0f, 0.0f, //14
	0.0f, 0.0f, //15
	
 };

const float piramVertexTangent[] = { 
	//Cara z = 1
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	
	//Cara z = -1				
	1.0f,	0.0f,	0.0f,		
	1.0f,	0.0f,	0.0f,		
	1.0f,	0.0f,	0.0f,		
	
	//Cara x = 1				
	0.0f,	0.0f,	-1.0f,		
	0.0f,	0.0f,	-1.0f,		
	0.0f,	0.0f,	-1.0f,		

	//Cara x = -1				
	0.0f,	0.0f,	1.0f,		
	0.0f,	0.0f,	1.0f,		
	0.0f,	0.0f,	1.0f,		
		
	//Cara y = -1					   
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
 };

#endif

//Marta Fern�ndez de la Mela Salcedo
