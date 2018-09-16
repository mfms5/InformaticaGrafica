#version 330 core

out vec4 outColor;

//4) Depurar primitivas. Pintar cada una de un color
// Variable predefinida que devuelve un entero, la primitiva a la que está asociada un fragmento
//gl_PrimitiveID

//Definir variable variante del color del vértice
in vec3 colorVert;

void main()
{
	outColor = vec4(colorVert,0);
	
}
