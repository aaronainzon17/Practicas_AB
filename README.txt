Instrucciones de compilacion:
• make

Instrucciones de ejecucion:
• tsp -[fb,av,pd,rp,rm] <nombre de fichero> [dimension]
• Opciones de ejecucion
    -fb 
        resolver mediante fuerza bruta el problema del viajante
        de comercio con la matriz de distancias especificada en <nombre de fichero>

    -av 
        resolver mediante algorimo voraz el problema del viajante
        de comercio con la matriz de distancias especificada en <nombre de fichero>

    -pd 
        resolver mediante programacion dinamica el problema del viajante
        de comercio con la matriz de distancias especificada en <nombre de fichero>

    -rp 
        resolver mediante ramificacion y poda el problema del viajante
        de comercio con la matriz de distancias especificada en <nombre de fichero>

    -rm <nombre de fichero> <dimension>
        generar matriz de diastancias aleatoria de dimension <dimension> que se 
        guarda en <nombre de fichero>

Instrucciones para probar el algoritmo:
• pruebas.sh

