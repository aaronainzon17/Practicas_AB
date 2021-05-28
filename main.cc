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
#define INF 2147483647 // Maximo valor de un entero

struct Recorrido{
    int distancia;
    vector<int> camino;
};

// Struct clave compuesta
struct PairKey
{
    int first;
    int sum;
 
    PairKey(int _first, vector<int> _second)
    {
        first=_first;
        sum=0;
        for (vector<int>::iterator it = _second.begin() ; it != _second.end(); ++it){
                    sum = sum + (1<<(*it));
        }
    }

    bool operator==(const PairKey &pk) const {
        return (first == pk.first) && (sum == pk.sum);
    }
};

// Struct Hash para el struct PairKey
struct pairKeyHash
{
    size_t operator() (const PairKey &clave) const
    {
        size_t hash_first = hash<int>()(clave.first);
        size_t hash_second = hash<int>()(clave.sum);
        return hash_first ^ hash_second;
    }
};

class Nodo {
public: 
    int ciudadActual;
    vector<int> camino;
    unordered_map<int, unordered_map<int,int>> matrizReducida;
    int coste;
    int nivel;
};

int reducirFila (unordered_map<int, unordered_map<int,int>> &matriz, int nFila, int nNodos){
    
    int min = (matriz[nFila])[1];
    for (int i = 2; i <=nNodos; i++){
        if ((matriz[nFila])[i] < min){
            min = matriz[nFila][i];
        }
    }
    if (min != INF){
        for (int i = 1; i <=nNodos; i++){
            if ((matriz[nFila])[i] != INF){
                (matriz[nFila])[i] -= min;
            }
        } 
        return min; 
    }else {
        return 0;
    }    
}

int reducirColumna (unordered_map<int, unordered_map<int,int>> &matriz, int nCol, int nNodos){
    int min = (matriz[1])[nCol];
    for (int i = 2; i <=nNodos; i++){
        if ((matriz[i])[nCol] < min){
            min = (matriz[i])[nCol];
        }
    }
    if (min != INF){
        for (int i = 1; i <=nNodos; i++){
            if ((matriz[i])[nCol] != INF){
                (matriz[i])[nCol] -= min; 
            }
        }        
        return min;
    }else {
        return 0;
    }
}

int coste (unordered_map<int, unordered_map<int,int>>  &matriz, int nNodos){
    int coste = 0;
    //Se reducen las filas de la matriz
    for (int i = 1; i <=nNodos; i++){
        coste += reducirFila(matriz,i,nNodos);
    }
    //Se reducen las columnas de la matriz (solamnete se reducen las que todavia estan sin reducir) 
    for (int j = 1; j <=nNodos; j++){
        coste += reducirColumna(matriz,j,nNodos);
    }
    return coste;
}

Nodo* crearNodo(unordered_map<int, unordered_map<int,int>> &matriz, vector<int> camino, int nivel, int origen, int destino, int nNodos){  
    Nodo* nodo = new Nodo;
    nodo->camino = camino;
    nodo->matrizReducida = matriz;

    if (nivel != 0){
        nodo->camino.push_back(destino);
        for (int i = 1; i <=nNodos; i++){
            nodo->matrizReducida[origen][i] = INF;
            nodo->matrizReducida[i][destino] = INF;
        }
    }else if (nivel == 0){
        for (int i = 1; i <=nNodos; i++){
            nodo->matrizReducida[i][i] = INF;
        }
    }

    nodo->matrizReducida[destino][1] = INF;
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

// Procedimiento de lectura de matriz de distancias
bool leerMatriz(const string nombreFichero, unordered_map<int, unordered_map<int,int>*> &matrizDistancias, int &nNodos){ //int matrizDistancias[nNodos][nNodos]
    // Abrimos el fichero de entrada
    ifstream f_entrada(nombreFichero);
    // Si el fichero de entrada se ha abierto correctamente
    if(f_entrada.is_open()){
        string line;
        // Leer linea de entrada
        nNodos = 0;
        while(getline(f_entrada,line)){
            // Separar distancias
            unordered_map<int,int>* lineDistance = new unordered_map<int,int>;
            istringstream iss(line);
            nNodos++;
            matrizDistancias[nNodos] = lineDistance;
            int column=1;
            for (int distance; iss >> distance; ){
                (*lineDistance)[column] = distance;
                column++;
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

void inicializarCandiatos( const int nNodos, vector<int> &candidatos){
    for (int i=2; i<=nNodos; i++){
        candidatos.push_back(i);
    }
}

void mostrarSolucion(vector<int>* camino, int &distancia, std::chrono::microseconds &tEjecucion){
    cout<< "Minimal tour length: " << distancia << endl;
    cout << "Itinerary: 1-";
    for (vector<int>::iterator it = camino->begin() ; it != camino->end(); ++it){
        if((*it) != 1){
            cout << *it << "-";
        }
    }
    double tEjec = tEjecucion.count();
    cout << "1\n";
    cout << "Tiempo de ejecucion: " <<  tEjec << " microsegundos" << endl;
}

// Procedimiento que resuelve por fuerza bruta el problema del viajante de comercio
void fuerzaBruta(unordered_map<int, unordered_map<int,int>*> &matrizDistancias,const int nNodos, vector<int>* &mejorCamino, int &mejorDistancia){
    vector<int> candidatos;
    inicializarCandiatos(nNodos,candidatos);
    int anterior;
    int distancia;
    mejorDistancia = 2147483647; // Maximo valor de un entero
    while(std::next_permutation(candidatos.begin(), candidatos.end())){
        vector<int>* camino = new vector<int>;
        camino->push_back(1);
        distancia = 0;
        for (vector<int>::iterator it = candidatos.begin() ; it != candidatos.end(); ++it){
            anterior=camino->back();
            camino->push_back(*it);
            distancia += (*matrizDistancias[anterior])[*it];
        }
        anterior=camino->back();
        distancia += (*matrizDistancias[anterior])[1];
        camino->push_back(1);
        if(distancia < mejorDistancia){
            mejorCamino = camino;
            mejorDistancia = distancia;
        }
    }
}


void algoritmoVoraz(unordered_map<int, unordered_map<int,int>*> &matrizDistancias,const int nNodos, vector<int>* &mejorCamino, int &mejorDistancia){
    bool visited[nNodos] = {false};
    int localMin; 
    int index = 1;
    int distancia = 0;
    vector<int>* camino = new vector<int>;
    for( int i = 0; i < nNodos - 1 ; i++)
    {
        visited[(index - 1)] = true;
        localMin = 2147483647; // Maximo valor de un entero
        for (auto a : *matrizDistancias[index])
        {
            if ((a.first != index) && (visited[a.first - 1] == false)){
                if(a.second < localMin)
                {
                    localMin = a.second;
                    index = a.first;
                }
            }
        }
        distancia += localMin; 
        camino->push_back(index);
    }
    visited[(index - 1)] = true;
    distancia += (*matrizDistancias[index])[1];
    mejorCamino = camino;
    mejorDistancia = distancia;
}

Recorrido programacionDinamicaPrima(unordered_map<int, unordered_map<int,int>*> &matrizDistancias, 
                                unordered_map<PairKey,Recorrido,pairKeyHash> &gtab,
                                int i, vector<int> S){
    Recorrido recorrido;
    int distancia;
    int actual;
    // Si no quedan ciudades por visitar
    if (S.empty()){
        // Se calcula la distancia de la ciudad en la que estamos al origen
        recorrido.distancia = (*matrizDistancias[i])[1];
        recorrido.camino.push_back(1);
        recorrido.camino.push_back(i);
    }else{
        // Se busca en gtab si ya hemos estado en esta ciudad con los mismos posibles destinos
        PairKey key(i,S);
        auto element = gtab.find(key);
        // Si hemos estado
        if (element != gtab.end()){
            // Se devuelve el mejor camino y distancias que estaban almacenados en gtab
            recorrido.camino = element->second.camino;
            recorrido.distancia= element->second.distancia;
        }
        // Si no hemos estado
        else{
            // Inicialmente distancia a infinito
            recorrido.distancia=INF;
            // Iteramos sobte los posibles destinos
            for (vector<int>::iterator j = S.begin() ; j != S.end(); ++j){
                // Dejamos al principio del vector 'S' de destinos la ciudad a la que vamos a ir 'j'
                actual = *j;
                *j = *S.begin();
                *S.begin() = actual;
                // Almacenamos en 'Sprima' el resto de ciudades, 
                // que seran los posibles destinos accesibles desde la ciudad a la que vamos a ir 'j'
                vector<int> Sprima(S.begin()+1,S.end());
                // Buscamos el mejor destino para i=j y S=Sprima
                Recorrido candidato = programacionDinamicaPrima(matrizDistancias,gtab,actual,Sprima);
                // Sumamos al recorrido actual la distancia de la ciudad actual 'i' a la ciudad a la que queremos ir
                // junto con la menor distancia del mejor camino para i=j y S=Sprima
                distancia = candidato.distancia + (*matrizDistancias[i])[actual];
                // Si es la mejor distancia obtenida
                if(distancia < recorrido.distancia){
                    recorrido.camino=candidato.camino;
                    recorrido.distancia=distancia;
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

Recorrido programacionDinamica(unordered_map<int, unordered_map<int,int>*> &matrizDistancias, int N){
        vector<int> S;
        inicializarCandiatos(N,S);
        unordered_map<PairKey,Recorrido,pairKeyHash> gtab;
        return programacionDinamicaPrima(matrizDistancias,gtab,1,S);
}

Nodo* ramificacionPoda(unordered_map<int, unordered_map<int,int>*> &matrizDistancias, int nNodos){
    vector<int>* aux;
    int poda = 0;
    algoritmoVoraz(matrizDistancias,nNodos,aux,poda);
    //Se crea una cola con prioridad de nodos vivos
    priority_queue<Nodo*,vector<Nodo*>,comp> nVivos;
    unordered_map<int, unordered_map<int,int>> m;
    //Se copia la matriz
    for (int i=1; i<=nNodos; i++){
        for (int j=1; j<=nNodos; j++){
            m[i][j] = (*matrizDistancias[i])[j];
        }
     }
    //Se crea el nodo raiz
    vector<int> camino;
    Nodo* raiz = crearNodo(m,camino,0,-1,1,nNodos);
    raiz->coste = coste(raiz->matrizReducida,nNodos);
    nVivos.push(raiz);

    while(!nVivos.empty()){
        Nodo* minimo = nVivos.top();
        nVivos.pop();
        int cuidadActual = minimo->ciudadActual;
        //Si se han visitado todas las ciudades
        if(minimo->nivel == (nNodos -1)){
            //COMO ESTA SIEMPRE ORDENADO POR COSTE, NO ES NECESARIA UNA FUCNION DE COTA YA QUE SIEMPRE 
            //SE VA A COGER EL DE MENOR COSTE
            return minimo;
        }
        for (int i = 1; i <=nNodos; i++){
            if(minimo->matrizReducida[cuidadActual][i] != INF){       
                Nodo* hijo = crearNodo(minimo->matrizReducida, minimo->camino, minimo->nivel + 1, cuidadActual, i, nNodos);
                hijo->coste = minimo->coste + minimo->matrizReducida[cuidadActual][i] + coste(hijo->matrizReducida,nNodos);
                if (hijo->coste <= poda){
                    nVivos.push(hijo);
                }else{
                    delete hijo;
                }
                
            }
        }
        delete minimo;
    }
    return 0;
}

int main(int argc, char *argv[] ){
    if(argc == 3){
        string opt = argv[1];
        string nombreFichero = argv[2];
        unordered_map<int, unordered_map<int,int>*> matrizDistancias;
        int nNodos, distancia;
        vector<int>* camino;
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
                camino = new vector<int>(solucion.camino);
            }else if(opt == "-rp"){ // Ramificacion y Poda
                Nodo* sol = ramificacionPoda(matrizDistancias,nNodos);
                camino = new vector<int>(sol->camino);
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
