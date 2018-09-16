#version 330 core

out vec4 outColor;

//4) Depurar primitivas. Pintar cada una de un color
//gl_PrimitiveID: Variable predefinida que devuelve un entero, la primitiva a la que está asociada un fragmento


void main()
{
	//outColor = vec4(gl_PrimitiveID*0.1f); //4) Cada primitiva se ve de un tono de gris distinto

	//Pintar sólo las pares de un color distinto (azul)
	if(mod (gl_PrimitiveID, 2)==0) {
	 outColor = vec4(0,0,1,0);
	}
	else {
	 outColor = vec4(1.0); //El resto se pintan blancas
	}
}
