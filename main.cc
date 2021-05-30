/********************************************************************************
/   Fichero: main.cc
/   Autores: Aaron Ibanez Ainzon (779088) y Pablo García García (781020)
/   Descripción: fichero main de las practicas de Algoritmia Basica
********************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <unordered_map>
#include <queue>

using namespace std;

// Valor maximo de un entero
#define INF 2147483647

/********************************************************
 *                                                      *
 *                                                      *
 *         FUNCIONES DE MANIPULACION DE DATOS           *
 *                                                      * 
 *                                                      *
 * *****************************************************/

//Pre:  nombreFichero respresenta el nombre del fichero que se va a leer
//
//Post: Si se ha podido leer el fichero se devuelve true y en
//      matrizDistancias se encontraran las distancias entre las ciudades
//      y en nNodos el numero de ciudades totales
//      En caso contrario se devolcera false
//
//Coste temporal  = O(N)
bool leerMatriz(const string nombreFichero, vector<int> &matrizDistancias, int &nNodos){ //int matrizDistancias[nNodos][nNodos]
    // Abrimos el fichero de entrada
    ifstream f_entrada(nombreFichero);
    // Si el fichero de entrada se ha abierto correctamente
    if(f_entrada.is_open()){
        string line;
        // Leer linea de entrada
        nNodos = 0;
        while(getline(f_entrada,line)){
            // Separar distancias
            istringstream iss(line);
            nNodos++;
            for (int distance; iss >> distance; ){
                matrizDistancias.push_back(distance);
            }
        }
        // Cerramos el fichero de entrada
        f_entrada.close();
    }else{
        return false;
    }
    return true;
}

//Pre:  matrizDistancias es un vector de enteros de tamaño nNodos * nNodos
//
//Post: Se muestra por pantalla la matriz
//
//Coste temporal  = O(N^2)
void mostrarMatriz(vector<int> &matrizDistancias, const int nNodos){
     for (int i=1; i<=nNodos; i++){
        for (int j=1; j<=nNodos; j++){
            cout << matrizDistancias[(i-1)*nNodos+j-1] << "\t" ;
        }
         cout << '\n';
     }
}

//Pre:  camino      = representa un vector ordenado de las ciudades por las que se ha pasado
//      distancia   = distancia recorrrida en el camino
//      tEjecucion  = tiempo de ejecucion del programa en us
//
//Post: Se muestam por pantalla los resultados
//
//Coste temporal  = O(N)
void mostrarSolucion(vector<int> camino, int &distancia, std::chrono::microseconds &tEjecucion){
    cout<< "Minimal tour length: " << distancia << endl;
    cout << "Itinerary: 1-";
    for (vector<int>::iterator it = camino.begin() ; it != camino.end(); ++it){
        if((*it) != 1){
            cout << *it << "-";
        }
    }
    double tEjec = tEjecucion.count();
    cout << "1\n";
    cout << "Tiempo de ejecucion: " <<  tEjec << " microsegundos" << endl;
}

//Pre:  nNodos es un entero mayor que 1
//
//Post: matrizDistancias conteine las distancias entre las N ciudades
//      generadas aleatoriamente
//
//Coste temporal  = O(N)
void alatoriaMatriz(const string nombreFichero, const int nNodos){
    // Abrimos el fichero de entrada
    ofstream f_salida(nombreFichero);
    // Si el fichero de entrada se ha abierto correctamente
    if(f_salida.is_open()){
        for (int i=1; i<=nNodos; i++){
            for (int j=1; j<=nNodos; j++){
                if(i==j){
                    f_salida << 0 << " ";
                }else{
                    f_salida << rand() %999+1 << " ";
                }
            }
            f_salida << "\n";
        }
        // Cerramos el fichero de entrada
        f_salida.close();
    }
}

/********************************************************
 *                                                      *
 *                                                      *
 *              FUNCIONES PARA FUERZA BRUTA             *
 *                                                      * 
 *                                                      *
 * *****************************************************/

//Pre:  nNodos entero mayor que 1
//
//Post: En sum se devuelve la suma de las ciudades como potencia de 2
//      En candidatos se devuelve un vector que conteiene todas las ciudades a 
//      las que se puede ir desde el nodo incial en un problema de dimension nNodos
//
//Coste temporal  = O(N)
void inicializarCandiatos( const int nNodos, vector<int> &candidatos){
    for (int i=2; i<=nNodos; i++){
        candidatos.push_back(i);
    }
}

//Pre:  nNodos  = respresenta el numero de ciudades
//      matrizDistancias = representa las distancias entre las distintas ciudades (debe de ser dimension nNodos * nNodos)
//
//Post: Devuelve el camino obtenido tras recorrer la matriz de distancias aplicando un algoritmo de fuerza bruta
//      mejorCamino  = vector que almacena el mejor camino encontrado
//      mejorDistancia = representa la mejor distancia enocntrada por el algoritmo
//
//Coste temporal  = O(n!)
void fuerzaBruta(vector<int> &matrizDistancias,const int nNodos, vector<int> &mejorCamino, int &mejorDistancia){
    vector<int> candidatos;
    // Se inicializa el vector con todas las posibles ciudades destino (todas menos la 1)
    inicializarCandiatos(nNodos,candidatos);
    int anterior;
    int distancia;
    // De momento, mejor distaria encontrada infinito
    mejorDistancia = INF;
    // Se prueban todas las permutaciones posibles del vector de candidatos
    while(std::next_permutation(candidatos.begin(), candidatos.end())){
        vector<int> camino;
        // Primera ciudad es la 1 siempre
        camino.push_back(1);
        // Se calcula la distancia total de la permitacion actual
        distancia = 0;
        for (vector<int>::iterator it = candidatos.begin() ; it != candidatos.end(); ++it){
            anterior=camino.back();
            camino.push_back(*it);
            // Distancia de los anteriores mas la de la ciudad anterior a la ciudad actual
            distancia += matrizDistancias[(anterior-1)*nNodos+(*it)-1];
        }
        // Distancia de los anteriores mas la de la ciudad anterior a 1
        anterior=camino.back();
        distancia += matrizDistancias[(anterior-1)*nNodos];
        // Ultima ciudad es la 1 siempre
        camino.push_back(1);
        // Si es la menor distancia que se ha encontrado
        if(distancia < mejorDistancia){
            // Se almacena el camino
            mejorCamino = camino;
            // Se almacena la distancia
            mejorDistancia = distancia;
        }
    }
}

/********************************************************
 *                                                      *
 *                                                      *
 *             FUNCIONES ALGORTIMO VORAZ                *
 *                                                      * 
 *                                                      *
 * *****************************************************/

//Pre:  nNodos  = respresenta el numero de ciudades
//      matrizDistancias = representa las distancias entre las distintas ciudades (debe de ser dimension nNodos * nNodos)
//
//Post: Devuelve el camino obtenido tras recorrer la matriz de distancias aplicando una heuristica voraz y su distancia
//      mejorCamino  = vector que almacena el mejor camino encontrado
//      mejorDistancia = representa la mejor distancia enocntrada por el algoritmo
//
//Coste temporal  = O(n^2)
void algoritmoVoraz(vector<int> &matrizDistancias,const int nNodos, vector<int> &mejorCamino, int &mejorDistancia){
    int localMin; 
    int dis;
    int indexAux;
    vector<int> camino;
    // Se parte de la ciudad 1
    int index = 1;
    // Inicialmente la distancia recorrida es 0
    int distancia = 0;
    // Inicialmente no se ha visitado ninguna ciudad
    bool visited[nNodos] = {false};
    // Nos quedan por visitar N-1 ciudades
    for( int i = 0; i < nNodos - 1 ; i++)
    {   
        // Marcar ciudad como visitada
        visited[(index - 1)] = true;
        // Minima distancia a la siguiente ciudad inicialmente infinita
        localMin = INF; 
        // Almacenamos la ciudad actual
        indexAux = index;
        // Buscar sobre todas las ciudades en las que no hemos estado
        for ( int j = 1; j <= nNodos; j++){
            if (!visited[j-1]){
                dis = matrizDistancias[(indexAux-1)*nNodos+j-1];
                // Si es la mas cercana
                if(dis < localMin)
                {
                    // Se almacena la distancia a la que esta
                    localMin = dis;
                    // Se almacena la ciudad
                    index = j;
                }
            }
        }
        // Se suma el minimo local encontrado a la distancia que ya habiamos recorrido
        distancia += localMin; 
        // Se añade al camino la ciudad a la que vamos
        camino.push_back(index);
    }
    // Se marca como visitada la ultima ciudad
    visited[(index - 1)] = true;
    // Se suma la disistancia de la ultima ciudad a 1 (que siempre es la ultima)
    distancia += matrizDistancias[(index-1)*nNodos];
    // Se devuelve el mejor camino encontrado
    mejorCamino = camino;
    // Se devuelve la mejor distancia encontrada
    mejorDistancia = distancia;
}

/********************************************************
 *                                                      *
 *                                                      *
 *         FUNCIONES PARA PROGRAMACION DINAMICA         *
 *                                                      * 
 *                                                      *
 * *****************************************************/

// Estructura de datos auxiliar que describe un recorrido
struct Recorrido{
    // Distancia recorrida
    int distancia;
    // Ciudades visitadas
    vector<int> camino;
};

// Estructura de datos clave compuesta
struct PairKey
{
    // Primera parte de la clave
    int first;
    // Segunda parte de la clave
    int second;
 
    //Pre:  _first   =   respresenta el primer elemento de la clave
    //      _second  =   representa el segundo elemento de la clave
    //Coste temporal  = O(1)
    PairKey(int _first, int _second)
    {
        first=_first;
        second=_second;
    }

    //Pre:  pk   = respresenta la clave con la que se quiere realizar la comparacion
    //
    //Post: Devuelve true si los dos atributos de la clave son iguales y false en caso contrario
    //
    //Coste temporal  = O(1)
    bool operator==(const PairKey &pk) const {
        return (first == pk.first) && (second == pk.second);
    }
};

// Estructura de datos para hacer un hash de una clase de tipo PairKey
struct pairKeyHash
{
    //Pre:  clave   = respresenta la clave a la que se le quiere aplicar el hash
    //
    //Post: Devuelve el hash del atributo clave de tipo PairKey
    //
    //Coste temporal  = O(1)
    size_t operator() (const PairKey &clave) const
    {
        size_t hash_first = hash<int>()(clave.first);
        size_t hash_second = hash<int>()(clave.second);
        return hash_first ^ hash_second;
    }
};

//Pre:  actual   = respresenta la ciudad a la que se quiere ir
//      S        = representa las ciudades a las que se puede ir
//
//Post: Devuelve true si la ciudad es visitable y false en caso contrario
//
//Coste temporal  = O(1)
bool ciudadVisitable(int actual, int S){
    return S & (1<<actual);
}


//Pre:  nNodos  = respresenta el numero de ciudades
//      matrizDistancias = representa las distancias entre las distintas ciudades (debe de ser dimension nNodos * nNodos)
//      i        = respresenta la ciudad actual
//      S        = representa las ciudades a las que se puede ir
//      gtab     = tabla hash de clave compuesta (i,S) que almacena el mejor Recorrido para cada tupla
//
//Post: Se devuelve una estructura de tipo Recorrido que incluye un vector ordenado con las ciudades recorridas y 
//      la distancia entera del camino recorrido
//
//Coste temporal  = O(n^2*2^n)
Recorrido programacionDinamicaPrima(vector<int> &matrizDistancias, 
                                unordered_map<PairKey,Recorrido,pairKeyHash> &gtab,
                                int i, int S, int nNodos){
    Recorrido recorrido;
    int distancia;
    int actual;
    int SAux;
    // Si no quedan ciudades por visitar
    if (S == 0){
        // Se calcula la distancia de la ciudad en la que estamos al origen
        recorrido.distancia = matrizDistancias[(i-1)*nNodos];
        recorrido.camino.push_back(1);
        recorrido.camino.push_back(i);
    }else{
        // Se busca en gtab si ya hemos estado en esta ciudad con los mismos posibles destinos
        PairKey key(i,S);
        auto element = gtab.find(key);
        // Si hemos estado
        if (element != gtab.end()){
            // Se devuelve el mejor camino y distancias que estaban almacenados en gtab
            return element->second;
        }
        // Si no hemos estado
        else{
            // Inicialmente distancia a infinito
            recorrido.distancia=INF;
            // Iteramos sobte los posibles destinos
            for (int iNext = 1; iNext<=nNodos; iNext++){
                if(ciudadVisitable(iNext,S)){
                    // Eliminamos la ciudad de las ciudades disponibles
                    SAux = S-(1<<iNext);
                    // Buscamos el mejor destino para iNext y SAux
                    Recorrido candidato = programacionDinamicaPrima(matrizDistancias,gtab,iNext,SAux,nNodos);
                    // Sumamos al recorrido del mejor destino para iNext y SAux 
                    // la distancia de la ciudad actual i a la ciudad a la que queremos ir iNext
                    distancia = candidato.distancia + matrizDistancias[(i-1)*nNodos+iNext-1];
                    // Si es la mejor distancia obtenida
                    if(distancia < recorrido.distancia){
                        recorrido.camino=candidato.camino;
                        recorrido.distancia=distancia;
                    }
                }
            }
            // Se añade la ciudad actual 'i' al camino
            recorrido.camino.push_back(i);
            // Se almacena el resultado en gtab para recuperarlo en las siguientes repeticiones
            // de esta combinacion
            gtab[key] = recorrido;
        }
    }
    return recorrido;
}

//Pre:  matrizDistancias   = representa las distancias entre las ciudades
//      N                  = representa la dimension del prblema como el numero de ciudades
//
//Post: Se devuelve una estructura de tipo Recorrido que incluye un vector ordenado con las ciudades recorridas y 
//      la distancia entera del camino recorrido
//
//Coste temporal  = O(n^2*2^n)
Recorrido programacionDinamica(vector<int> &matrizDistancias, int N){
        // Declaracion de la tabla hash de Recorridos de clave compuesta (i,S)
        unordered_map<PairKey,Recorrido,pairKeyHash> gtab;
        // Valor inicial de S
        int S = (1<<(N+1))-4;
        // Invocacion con (1,S)
        return programacionDinamicaPrima(matrizDistancias,gtab,1,S,N);
}

/********************************************************
 *                                                      *
 *                                                      *
 *         FUNCIONES PARA RAMIFICACION Y PODA           *
 *                                                      * 
 *                                                      *
 * *****************************************************/

//Clase que define un nodo 
class Nodo {
public: 
    int ciudadActual;
    vector<int> camino;
    vector<int> matrizReducida;
    int coste;
    int nivel;
};

// Pre:  nFila   = respresenta fila de la matriz de cuidades que se quiere reducir
//       nNodos  = respresenta el numero de ciudades
//       matriz  = vector que almacena la distancias entre las ciudades
//
// Post: Devuelve el valor de la reduccion de la fila y la matriz de distancias resultante
//
// Coste temporal  = O(n^2)
int reducirFila (vector<int> &matriz, int nFila, int nNodos){
    
    int finFila = nFila*(nNodos+1);
    int min = matriz[nFila*nNodos];
    for (int i = 1; i < nNodos; i++){
        if (matriz[(nNodos*nFila) + i] < min){
            min = matriz[(nNodos*nFila) + i];
        }
    }
    if (min != INF && min != 0){
        for (int i = 0; i < nNodos; i++){
            if (matriz[(nNodos*nFila) + i] != INF){
                matriz[(nNodos*nFila) + i] -= min;
            }
        } 
        return min; 
    }else {
        return 0;
    }    
}


// Pre:  nCol    = respresenta columna de la matriz de cuidades que se quiere reducir
//       nNodos  = respresenta el numero de ciudades
//       matriz  = vector que almacena la distancias entre las ciudades
//
// Post: Devuelve el valor de la reduccion de la columna y la matriz de distancias resultante
//
// Coste temporal  = O(n^2)
int reducirColumna (vector<int> &matriz, int nCol, int nNodos){
    int min = matriz[nCol];
    for (int i = 1; i < nNodos; i++){
        if (matriz[i*nNodos + nCol] < min){
            min = matriz[i*nNodos + nCol];
        }
    }
    if (min != INF && min != 0){
        for (int i = 0; i < nNodos; i++){
            if (matriz[i*nNodos + nCol] != INF){
                matriz[i*nNodos + nCol] -= min; 
            }
        }        
        return min;
    }else {
        return 0;
    }
}

// Pre:  nNodos  = respresenta el numero de ciudades
//       matriz  = vector que almacena la distancias entre las ciudades
//
// Post: Devuelve coste de la reduccion de la matriz
//
// Coste temporal  = O(n^2)
int coste (vector<int>  &matriz, int nNodos){
    int coste = 0;
    //Se reducen las filas de la matriz
    for (int i = 0; i < nNodos; i++){
        coste += reducirFila(matriz,i,nNodos);
    }
    //Se reducen las columnas de la matriz (solamnete se reducen las que todavia estan sin reducir) 
    for (int j = 0; j < nNodos; j++){
        coste += reducirColumna(matriz,j,nNodos);
    }
    return coste;
}

// Pre:  camino  = camino recorrrido hasta llegar a ese nodo
//       origen  = ciudad de donde venia el nodo padre
//       destino = ciudad a la que corresponde el nodo 
//       nivel   = nivel del arbol al que se encuentra ese nodo
//       nNodos  = respresenta el numero de ciudades
//       matriz  = vector que almacena la distancias entre las ciudades
//
// Post: Devuelve un objeto de tipo Nodo con la matriz de distancias reducidas y la fila y columna
//       correspondiente a infinito
//        
// Coste temporal  = O(n^2)
Nodo* crearNodo(vector<int> &matriz, vector<int> camino, int nivel, int origen, int destino, int nNodos){  
    Nodo* nodo = new Nodo;
    nodo->camino = camino;
    nodo->matrizReducida = matriz;

    if (nivel != 0){
        nodo->camino.push_back(destino);
        for (int i = 0; i < nNodos; i++){
            nodo->matrizReducida[(origen - 1)*nNodos + i] = INF;
            nodo->matrizReducida[(destino - 1)+ i*nNodos] = INF;
        }
    }else if (nivel == 0){
        for (int i = 0; i < nNodos; i++){
            nodo->matrizReducida[i*nNodos + i] = INF;
        }
    }

    nodo->matrizReducida[(destino - 1)*nNodos] = INF;
    nodo->nivel = nivel;
    nodo->ciudadActual = destino; 
    return nodo;
}

// Clase utlilizada para comparar los nodos
class comp {
public:
    //Pre:  nodo1   = representa un objeto de tipo Nodo 
    //      nodo2   = respresenta un objeto de tipo Nodo
    //
    //Post: Devuelve true si el coste del nodo1 es mayor al coste del nodo2
    //
    //Coste temporal  = O(1)
    bool operator()(const Nodo* nodo1, const Nodo* nodo2) const {
        return nodo1->coste > nodo2->coste;
    }
};

//Pre:  nNodos   = numero de ciudades
//
//Post: Devuelve un objeto de clase Nodo, el cual contiene un camino optimo para el 
//      problema y el coste del recorrido
//
//Coste temporal  = O(n^2*2^n)
Nodo* ramificacionPoda(vector<int> &matrizDistancias, int nNodos){
    vector<int> aux;
    int poda = 0;
    int nodos_totales = 0;
    int nodos_podados = 0;
    //Se utiliza el algoritmo voraz para encontar una solucion suboptima y establecer un 
    //valor para la poda
    algoritmoVoraz(matrizDistancias,nNodos,aux,poda);
    //Se crea una cola con prioridad de nodos vivos donde en la primera posicion se encuentra
    //el nodo de menor coste
    priority_queue<Nodo*,vector<Nodo*>,comp> nVivos;
    vector<int> camino;
    //Se crea el nodo raiz y se añade a la lista de nodos vivos
    Nodo* raiz = crearNodo(matrizDistancias,camino,0,-1,1,nNodos);
    raiz->coste = coste(raiz->matrizReducida,nNodos);
    nVivos.push(raiz);
    while(!nVivos.empty()){
        //Se extrae de la cola de nodos vivos el nodo de menor coste
        Nodo* minimo = nVivos.top();
        nVivos.pop();
        int cuidadActual = minimo->ciudadActual;
        //Si el nodo extraido ha visitado todas las ciudades
        if(minimo->nivel == (nNodos -1)){
            cout << "El numero de nodos totales generados es: " << nodos_totales << endl;
            cout << "El numero de nodos podados es: " << nodos_podados << endl;
            //Se devuelve el nodo como solucion optima
            return minimo;
        }else{
            //Si el nodo no es solucion se generan sus hijos
            for (int i = 0; i < nNodos; i++){
                // Se comprueba que el camino a ese nodo existe 
                if(minimo->matrizReducida[(cuidadActual-1)*nNodos + i] != INF){
                    nodos_totales++;
                    //Se crea el nodo y se evalua su coste     
                    Nodo* hijo = crearNodo(minimo->matrizReducida, minimo->camino, minimo->nivel + 1, cuidadActual, i + 1, nNodos);
                    hijo->coste = minimo->coste + minimo->matrizReducida[(cuidadActual-1)*nNodos + i] + coste(hijo->matrizReducida,nNodos);
                    //Si el coste es menor que la funcion de poda se introduce a la lista de nodos vivos, si no, se poda
                    if (hijo->coste <= poda){
                        nVivos.push(hijo);
                    }else{
                        nodos_podados++;
                        delete hijo;
                    }
                }
            }
        }
        //Se elimina el nodo extraido de la cola
        delete minimo;
    }
    return 0;
}

/********************************************************
 *                                                      *
 *                                                      *
 *                       MAIN                           *
 *                                                      * 
 *                                                      *
 * *****************************************************/

int main(int argc, char *argv[] ){
    // El programa tiene 2 argumentos
    if(argc == 3){
        vector<int> matrizDistancias, camino;
        int nNodos, distancia;
        std::chrono::microseconds tEjecucion;
        // Opcion introducida
        string opt = argv[1];
        // Fichero introducido
        string nombreFichero = argv[2];
        // Si se ha podido leer la matriz de distancias
        if(leerMatriz(nombreFichero, matrizDistancias,nNodos)){
            // Empezar a contar el tiempo de ejecucion del algoritmo
            std::chrono::steady_clock::time_point start = chrono::steady_clock::now();
            // Fuerza Bruta
            if (opt == "-fb"){
                fuerzaBruta(matrizDistancias,nNodos,camino,distancia);       
            // Algortimo Voraz
            }else if(opt == "-av"){ 
                algoritmoVoraz(matrizDistancias,nNodos,camino,distancia);
            // Programacion Dinamica
            }else if(opt == "-pd"){ 
                Recorrido solucion = programacionDinamica(matrizDistancias,nNodos);
                distancia = solucion.distancia;
                // Esta solcuion devuelve el camino del reves por su estructura 
                // recursiva por lo que hay que darle la vuelta
                reverse(solucion.camino.begin(),solucion.camino.end());
                camino = solucion.camino;
            // Ramificacion y Poda
            }else if(opt == "-rp"){
                Nodo* sol = ramificacionPoda(matrizDistancias,nNodos);
                camino = sol->camino;
                distancia = sol->coste;
            // Opcion desconocida
            }else{
                cout << "Opcion " << opt << " es invalida -> tsp -[fb,av,pd,rp] <nombre de fichero>" << endl;    
                return -1;
            }
            // Parar de contar el tiempo de ejecucion
            std::chrono::steady_clock::time_point end = chrono::steady_clock::now();
            tEjecucion = chrono::duration_cast<chrono::microseconds>(end - start);
            // Mostrar la solucion encontrada
            mostrarSolucion(camino,distancia,tEjecucion);
        }
        // Si no se pudo leer la matriz de confusion
        else{
            cout << "No se pudo abrir el fichero " << nombreFichero << " -> tsp -[fb,av,pd,rp,rm] <nombre de fichero> [dimension]" << endl;    
            return -1;    
        }
    }
    else if (argc == 4){
        int nNodos, distancia;
        // Opcion introducida
        string opt = argv[1];
        // Fichero introducido
        string nombreFichero = argv[2];
        // Dimension del problema
        nNodos = stoi(argv[3]);
        // Random matrix
        if(opt == "-rm"){
            alatoriaMatriz(nombreFichero,nNodos);
        // Opcion desconocida
        }else{
            cout << "Opcion " << opt << " es invalida -> tsp -[fb,av,pd,rp] <nombre de fichero> [dimension]" << endl;    
            return -1;
        }
    }
    else{
        cout << "Numero de argumentos incorrecto -> tsp -[fb,av,pd,rp,rm] <nombre de fichero> [dimension]" << endl;    
        return -1;
    }
    return 0;
}
