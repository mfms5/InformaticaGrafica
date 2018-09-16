#version 330 core

in vec3 inPos; //Ya está en coordenadas proyectadas
out vec2 texCoord;

void main()
{
	texCoord = inPos.xy*0.5+vec2(0.5); //Coordenadas de textrua, cambia el rango de (-1,1) a (0,1)
	gl_Position =  vec4 (inPos,1.0); //Vértice proyectado

}
