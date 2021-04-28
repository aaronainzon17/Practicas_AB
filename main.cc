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
#include <unordered_map>

using namespace std;

// Procedimiento de lectura de matriz de distancias
bool leerMatriz(string nombreFichero, vector<vector<int>*> &matrizDistancias, vector<int> &nodos){
    // Abrimos el fichero de entrada
    ifstream f_entrada(nombreFichero);
    // Si el fichero de entrada se ha abierto correctamente
    if(f_entrada.is_open()){
        int nodo = 1;
        string line;
        // Leer linea de entrada
        while(getline(f_entrada,line)){
            // Separar distancias
            vector<int>* lineDistance = new vector<int>;
            istringstream iss(line);
            matrizDistancias.push_back(lineDistance);
            for (int distance; iss >> distance; ){
                lineDistance->push_back(distance);
            }
            nodos.push_back(nodo);
            nodo++;
        }
        // Cerramos el fichero de entrada
        f_entrada.close();
    }else{
        return false;
    }
    return true;
}

// Procedimiento para mostrar la matriz de distancias
void mostrarMatriz(vector<vector<int>*> &matrizDistancias){
     for (vector<vector<int>*>::iterator itRows = matrizDistancias.begin() ; itRows != matrizDistancias.end(); ++itRows){
         for (vector<int>::iterator itColumns = (*itRows)->begin() ; itColumns != (*itRows)->end(); ++itColumns){
            std::cout << *itColumns << ' ';
         }
         std::cout << '\n';
     }
}


// Procedimiento que resuelve por fuerza bruta el problema del viajante de comercio
void fuerzaBruta(vector<vector<int>*> &matrizDistancias, vector<int> &nodos, int &distancia, vector<int> &solucion){
    int row, column=0;
    for (vector<vector<int>*>::iterator itRows = matrizDistancias.begin() ; itRows != matrizDistancias.end(); ++itRows){
        row=0;
         for (vector<int>::iterator itColumns = (*itRows)->begin() ; itColumns != (*itRows)->end(); ++itColumns){
            std::cout << *itColumns << ' ';
         }
         std::cout << '\n';
     }
}

void algoritmoVoraz(vector<vector<int>*> &matrizDistancias){

}

void programacionFinamica(vector<vector<int>*> &matrizDistancias){

}

void ramificacionPoda(vector<vector<int>*> &matrizDistancias){

}

int main(int argc, char *argv[] ){
    if(argc == 3){
        string opt = argv[1];
        string nombreFichero = argv[2];
        vector<vector<int>*> matrizDistancias;
        vector<int> nodos;
        vector<int> solucion;
        int distancia;
        leerMatriz(nombreFichero, matrizDistancias,nodos);
        //mostrarMatriz(matrizDistancias);
        if (opt == "-fb"){      // Fuerza Bruta
            fuerzaBruta(matrizDistancias,nodos,distancia,solucion);        
        }else if(opt == "-av"){ // Algortimo Voraz
            algoritmoVoraz(matrizDistancias);
        }else if(opt == "-pd"){ // Programacion Dinamica
            programacionFinamica(matrizDistancias);
        }else if(opt == "-rp"){ // Ramificacion y Poda
            ramificacionPoda(matrizDistancias);
        }else{
            cout << "Opcion invalida -> tsp -[fb,av,pd,rp] <nombre de fichero>" << endl;    
            return -1;
        }
    }
    else{
        cout << "Numero de parametros incorrecto -> tsp -[fb,av,pd,rp] <nombre de fichero>" << endl;    
        return -1;
    }
    return 0;
}
