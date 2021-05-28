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
    //Coms: Coste temporal  = O(1)
    //      Coste memoria   = O(1)
    PairKey(int _first, int _second)
    {
        first=_first;
        second=_second;
    }

    //Pre:  pk   = respresenta la clave con la que se quiere realizar la comparacion
    //Post: Devuelve true si los dos atributos de la clave son iguales y false en caso contrario
    //Coms: Coste temporal  = O(1)
    //      Coste memoria   = O(1)
    bool operator==(const PairKey &pk) const {
        return (first == pk.first) && (second == pk.second);
    }
};

// Estructura de datos para hacer un hash de una clase de tipo PairKey
struct pairKeyHash
{
    //Pre:  clave   = respresenta la clave a la que se le quiere aplicar el hash
    //Post: Devuelve el hash del atributo clave de tipo PairKey
    //Coms: Coste temporal  = O(1)
    //      Coste memoria   = O(1)
    size_t operator() (const PairKey &clave) const
    {
        size_t hash_first = hash<int>()(clave.first);
        size_t hash_second = hash<int>()(clave.second);
        return hash_first ^ hash_second;
    }
};
//Clase que define un nodo 
class Nodo {
public: 
    int ciudadActual;
    vector<int> camino;
    vector<int> matrizReducida;
    int coste;
    int nivel;
};

/*
* Pre:  nFila   = respresenta fila de la matriz de cuidades que se quiere reducir
*       nNodos  = respresenta el numero de ciudades
*       matriz  = vector que almacena la distancias entre las ciudades
* Post: Devuelve el valor de la reduccion de la fila y la matriz de distancias resultante
* Coms: Coste temporal  = O(n^2)
*       Coste memoria   = 
*/
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

/*
* Pre:  nCol    = respresenta columna de la matriz de cuidades que se quiere reducir
*       nNodos  = respresenta el numero de ciudades
*       matriz  = vector que almacena la distancias entre las ciudades
* Post: Devuelve el valor de la reduccion de la columna y la matriz de distancias resultante
* Coms: Coste temporal  = O(n^2)
*       Coste memoria   = 
*/
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

class comp {
public:
    bool operator()(const Nodo* nodo1, const Nodo* nodo2) const {
        return nodo1->coste > nodo2->coste;
    }
};

//Pre:  nombreFichero   = respresenta el nombre del fichero que se va a leer
//Post: Si se ha podido leer el fichero se devuelve true y en
//      matrizDistancias se encontraran las distancias entre las ciudades
//      y en nNodos el numero de ciudades totales
//      En caso contrario se devolcera false
//Coms: Coste temporal  = O(N)
//      Coste memoria   = O(N)
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

// Procedimiento para mostrar la matriz de distancias
void mostrarMatriz(unordered_map<int, unordered_map<int,int>*> &matrizDistancias, const int nNodos){
     for (int i=1; i<=nNodos; i++){
        for (int j=1; j<=nNodos; j++){
            cout << (*matrizDistancias[i])[j] << " " ;
        }
         cout << '\n';
     }
}

void inicializarCandiatos( const int nNodos, vector<int> &candidatos, int &sum){
    sum = 0;
    for (int i=2; i<=nNodos; i++){
        candidatos.push_back(i);
        sum = sum + (1<<i);
    }
}

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

// Procedimiento que resuelve por fuerza bruta el problema del viajante de comercio
void fuerzaBruta(vector<int> &matrizDistancias,const int nNodos, vector<int> &mejorCamino, int &mejorDistancia){
    vector<int> candidatos;
    int sum;
    inicializarCandiatos(nNodos,candidatos,sum);
    int anterior;
    int distancia;
    mejorDistancia = 2147483647; // Maximo valor de un entero
    while(std::next_permutation(candidatos.begin(), candidatos.end())){
        vector<int> camino;
        camino.push_back(1);
        distancia = 0;
        for (vector<int>::iterator it = candidatos.begin() ; it != candidatos.end(); ++it){
            anterior=camino.back();
            camino.push_back(*it);
            distancia += matrizDistancias[(anterior-1)*nNodos+(*it)-1];
        }
        anterior=camino.back();
        distancia += matrizDistancias[(anterior-1)*nNodos];
        camino.push_back(1);
        if(distancia < mejorDistancia){
            mejorCamino = camino;
            mejorDistancia = distancia;
        }
    }
}


void algoritmoVoraz(vector<int> &matrizDistancias,const int nNodos, vector<int> &mejorCamino, int &mejorDistancia){
    bool visited[nNodos] = {false};
    int localMin; 
    int index = 1;
    int distancia = 0;
    int dis;
    int indexAux;
    vector<int> camino;
    for( int i = 0; i < nNodos - 1 ; i++)
    {
        visited[(index - 1)] = true;
        localMin = INF; // Maximo valor de un entero
        indexAux = index;
        for ( int j = 1; j <= nNodos; j++){
            if (!visited[j-1]){
                dis = matrizDistancias[(indexAux-1)*nNodos+j-1];
                if(dis < localMin)
                {
                    localMin = dis;
                    index = j;
                }
            }
        }
        distancia += localMin; 
        camino.push_back(index);
    }
    visited[(index - 1)] = true;
    distancia += matrizDistancias[(index-1)*nNodos];
    mejorCamino = camino;
    mejorDistancia = distancia;
}

//Pre:  actual   = respresenta la ciudad a la que se quiere ir
//      S        = representa las ciudades a las que se puede ir
//Post: Devuelve true si la ciudad es visitable y false en caso contrario
//Coms: Coste temporal  = O(1)
//      Coste memoria   = O(1)
bool ciudadVisitable(int actual, int S){
    return S & (1<<actual);
}

//Pre:  actual   = respresenta la ciudad a la que se quiere ir
//      S        = representa las ciudades a las que se puede ir
//Post: Devuelve el tamaño del camino hamiltoniano de menor recorrido comenzando en el nodo 1
//      y el tiempo tardado
//Coms: Coste temporal  = O(n^2*2^n)
//      Coste memoria   =
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

Recorrido programacionDinamica(vector<int> &matrizDistancias, int N){
        vector<int> S;
        int sum;
        inicializarCandiatos(N,S,sum);
        unordered_map<PairKey,Recorrido,pairKeyHash> gtab;
        return programacionDinamicaPrima(matrizDistancias,gtab,1,sum,N);
}
//Pre:  nNodos   = numero de ciudades
//Post: Devuelve un objeto de clase Nodo, el cual contiene un camino optimo para el 
//      problema y el coste del recorrido
//Coms: Coste temporal  = O(n^2*2^n)
//      Coste memoria   =
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
    vector<int> m = matrizDistancias;
    vector<int> camino;
    //Se crea el nodo raiz y se añade a la lista de nodos vivos
    Nodo* raiz = crearNodo(m,camino,0,-1,1,nNodos);
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

int main(int argc, char *argv[] ){
    if(argc == 3){
        string opt = argv[1];
        string nombreFichero = argv[2];
        unordered_map<int, unordered_map<int,int>*> matrizDistancias2;
        vector<int> matrizDistancias;
        int nNodos, distancia;
        vector<int> camino;
        std::chrono::microseconds tEjecucion;
        if(leerMatriz(nombreFichero, matrizDistancias,nNodos)){
            std::chrono::steady_clock::time_point start = chrono::steady_clock::now();
            if (opt == "-fb"){      // Fuerza Bruta
                fuerzaBruta(matrizDistancias,nNodos,camino,distancia);       
            }else if(opt == "-av"){ // Algortimo Voraz
                algoritmoVoraz(matrizDistancias,nNodos,camino,distancia);
            }else if(opt == "-pd"){ // Programacion Dinamica
                Recorrido solucion = programacionDinamica(matrizDistancias,nNodos);
                distancia = solucion.distancia;
                reverse(solucion.camino.begin(),solucion.camino.end());
                camino = solucion.camino;
            }else if(opt == "-rp"){ // Ramificacion y Poda
                Nodo* sol = ramificacionPoda(matrizDistancias,nNodos);
                camino = sol->camino;
                distancia = sol->coste;
            }else{
                cout << "Opcion " << opt << " es invalida -> tsp -[fb,av,pd,rp] <nombre de fichero>" << endl;    
                return -1;
            }
            std::chrono::steady_clock::time_point end = chrono::steady_clock::now();
            tEjecucion = chrono::duration_cast<chrono::microseconds>(end - start);
            mostrarSolucion(camino,distancia,tEjecucion);
        }
        else{
            cout << "No se pudo abrir el fichero " << nombreFichero << " -> tsp -[fb,av,pd,rp] <nombre de fichero>" << endl;    
            return -1;    
        }
    }
    else{
        cout << "Numero de parametros incorrecto -> tsp -[fb,av,pd,rp] <nombre de fichero>" << endl;    
        return -1;
    }
    return 0;
}
