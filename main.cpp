/********************************************************************************
/   Fichero: main.cpp
/   Autores: Aaron Ibanez Ainzon (779088) y Pablo García García (781020)
/   Descripción: fichero main de las practicas de Algoritmia Basica
********************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// Funcion para el tratamiento de una petición del tipo LISTADO TOTAL
void leerMatriz(string nombreFichero, vector<vector<int>>* matrizDistancias){
    // Abrimos el fichero de entrada
    ifstream f_entrada(nombreFichero);
    // Si el fichero de entrada se ha abierto correctamente
    if(f_entrada.is_open()){
        int distancia;
        string line;
        // Leer linea de entrada
        while(getline(f_entrada,line)){
            // Separar distancias
            vector<int> lineDistance;
            istringstream iss(line);
            for (int distance; iss >> distance; )
                lineDistance.push_back(distance);
            }
            matrizDistancias->push_back(lineDistance);
    }
    // Cerramos el fichero de entrada
    f_entrada.close();
}

void bruteForce(vector<vector<int>>* matrizDistancias){

}

int main(int argc, char *argv[] ){
    if(argc == 3){
        string opt = argv[1];
        string nombreFichero = argv[2];
        vector<vector<int>> matrizDistancias;
        leerMatriz(nombreFichero, &matrizDistancias);
        if (opt == "-fb"){
            bruteForce(&matrizDistancias);
        }else if(opt == "-av"){

        }else if(opt == "-pd"){

        }else if(opt == "-rp"){
            
        }else{
            cout << "Uso incorrecto -> tsp -[fb,av,pd,rp] <nombre de fichero>" << endl;    
            return -1;
        }
    }
    else{
        cout << "Uso incorrecto -> tsp -[fb,av,pd,rp] <nombre de fichero>" << endl;    
        return -1;
    }
    return 0;
}
