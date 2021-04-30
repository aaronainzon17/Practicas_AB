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

using namespace std;

// Procedimiento de lectura de matriz de distancias
bool leerMatriz(const string nombreFichero, unordered_map<int, unordered_map<int,int>*> &matrizDistancias, int &nNodos){
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
            //mostrarSolucion(&camino,distancia);
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
            if ((a.first != index) && (a.first != 1) && (visited[a.first - 1] == false)){
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

void programacionFinamica(unordered_map<int, unordered_map<int,int>*> &matrizDistancias){

}

void ramificacionPoda(unordered_map<int, unordered_map<int,int>*> &matrizDistancias){

}

int main(int argc, char *argv[] ){
    if(argc == 3){
        string opt = argv[1];
        string nombreFichero = argv[2];
        std::chrono::microseconds tEjecucion;
        unordered_map<int, unordered_map<int,int>*> matrizDistancias;
        int nNodos;
        vector<int>* camino;
        int distancia;
        leerMatriz(nombreFichero, matrizDistancias,nNodos);
        //mostrarMatriz(matrizDistancias,nNodos);
        std::chrono::steady_clock::time_point start = chrono::steady_clock::now();
        if (opt == "-fb"){      // Fuerza Bruta
            fuerzaBruta(matrizDistancias,nNodos,camino,distancia);       
        }else if(opt == "-av"){ // Algortimo Voraz
            algoritmoVoraz(matrizDistancias,nNodos,camino,distancia);
        }else if(opt == "-pd"){ // Programacion Dinamica
            programacionFinamica(matrizDistancias);
        }else if(opt == "-rp"){ // Ramificacion y Poda
            ramificacionPoda(matrizDistancias);
        }else{
            cout << "Opcion invalida -> tsp -[fb,av,pd,rp] <nombre de fichero>" << endl;    
            return -1;
        }
        std::chrono::steady_clock::time_point end = chrono::steady_clock::now();
        tEjecucion = chrono::duration_cast<chrono::microseconds>(end - start);
        mostrarSolucion(camino,distancia,tEjecucion);
    }
    else{
        cout << "Numero de parametros incorrecto -> tsp -[fb,av,pd,rp] <nombre de fichero>" << endl;    
        return -1;
    }
    return 0;
}
