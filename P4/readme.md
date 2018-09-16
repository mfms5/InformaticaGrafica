## Práctica 4: Aumento de realismo – Post-Proceso (MRT)

El objetivo de esta práctica es que el alumno entienda que los métodos de iluminación local no pueden simular muchos de los comportamientos típicos de la luz. La posibilidad de renderizar sobre una textura que se pueda utilizar posteriormente en la escena nos permite diseñar técnicas que suplan de forma más o menos efectiva alguna de estas carencias.
Se utilizará la librería auxiliar GLUT para inicializar el contexto de OpenGL, crear el Frame Buffer, crear la ventana de renderizado y definir las funciones encargadas de tratar los eventos enviados por el sistema operativo. Además, se utilizará la librería GLEW para inicializar las extensiones. 

Funciones que se implementan:

-Gaussian blur.
-Motion blur.
-Depth of field
-Controlar los parámetros del Motion Blur a través de teclado. 
<p align="center">
  <img width="40%" height="40%" src="https://raw.githubusercontent.com/mfms5/InformaticaGrafica/master/P4/capturas/motionblur1.png">
  <img width="40%" height="40%" src="https://raw.githubusercontent.com/mfms5/InformaticaGrafica/master/P4/capturas/motionblur2.png">
</p>

-Controlar los parámetros del DOF por teclado (distancia focal y distancia de desenfoque máximo). 
<p align="center">
  <img width="40%" height="40%" src="https://raw.githubusercontent.com/mfms5/InformaticaGrafica/master/P4/capturas/pulsarHaumentarDesenfoque.png">
</p>

-Utilizar el buffer de profundidad para controlar el DOF. 
<p align="center">
  <img width="60%" height="60%" src="https://raw.githubusercontent.com/mfms5/InformaticaGrafica/master/P4/capturas/aumentarVSdisminuriDistanciaFocal.png">
</p>

-Subir nuevas máscaras de convolución a través de variables uniform. Seleccionar entre varias máscaras a través de teclado. 
<p align="center">
  <img width="40%" height="40%" src="https://raw.githubusercontent.com/mfms5/InformaticaGrafica/master/P4/capturas/mascConv_bordes.png">
  <img width="40%" height="40%" src="https://raw.githubusercontent.com/mfms5/InformaticaGrafica/master/P4/capturas/mascConv_gaussiano.png">
</p>
