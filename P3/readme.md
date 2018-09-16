## Práctica 3: Introducción a la programación en OpenGL

El objetivo de esta práctica es que el alumno programe la etapa de aplicación utilizando el API de OpenGL 3.3, utilizando las librerías GLUT y GLEW.

Funcionalidades que se implementan:

-Modificar las propiedades (intensidad y posición) de la luz a través del teclado. 
<p align="center">
  <img width="30%" height="30%" src="https://raw.githubusercontent.com/mfms5/InformaticaGrafica/master/P3/capturas/luzTeclado.png">
  <img width="30%" height="30%" src="https://raw.githubusercontent.com/mfms5/InformaticaGrafica/master/P3/capturas/luzTeclado2.png">
</p>

-Definer una matriz de proyección que conserve el aspect ratio cuando cambiamos el tamaño de la ventana. 
<p align="center">
  <img width="30%" height="30%" src="https://raw.githubusercontent.com/mfms5/InformaticaGrafica/master/P3/capturas/ar1.png">
  <img width="50%" height="50%" src="https://raw.githubusercontent.com/mfms5/InformaticaGrafica/master/P3/capturas/ar2.png">
</p>

-Añadir un nuevo cubo a la escena. El segundo cubo deberá orbitar alrededor del primero describiendo una circunferencia a la vez que rota sobre su eje Y. NOTA: No tienes por que crear otro objeto. Pinta el mismo VAO con otra matriz model. 
<p align="center">
  <img width="30%" height="30%" src="https://raw.githubusercontent.com/mfms5/InformaticaGrafica/master/P3/capturas/nuevocubo.png">
</p>

-Control de la cámara con el teclado. Controles mínimos que deberán incluirse: movimiento hacia adelante, retroceso, movimientos laterales (izquierda y derecha) y giros (izquierda y derecha). Importante: la posición de la luz debe ser invariante con respecto a la posición de la cámara. 
<p align="center">
  <img width="30%" height="30%" src="https://raw.githubusercontent.com/mfms5/InformaticaGrafica/master/P3/capturas/movercam.png">
</p>

-Mejorar el comportamiento de las texturas utilizando un filtro anisotrópico.



