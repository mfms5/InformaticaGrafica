#version 330 core

out vec4 outColor;

//5)Definir variable variante del color del vértice
in vec3 colorVert;

void main()
{
	outColor = vec4(colorVert,0);
	
}
