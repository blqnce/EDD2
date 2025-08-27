#include "fasta.h"
#include <iostream>
#include <string>

using namespace std;

//Funciones de Secuencia

string Secuencia::getDescripcion(){
    return descripcion;
}

vector<string> Secuencia::getDatos(){
    return datos;
}

int Secuencia::getAncho(){
    return ancho;
}

//Funciones de Fasta

vector<Secuencia> Fasta::getFasta(){
    return fasta;
}

Fasta Fasta::cargar(string nombre_entrada){

    Secuencia sec;
    Fasta fas;
    string temp;
    vector<string> entrada;
    vector<Secuencia> sec_entrada;
    ifstream archivo(nombre_entrada);
    bool encontrado = false;
    char letra;
    

    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << nombre_entrada << endl;
        return fas;
    }

    

    while(getline(archivo, temp)){
        if(encontrado == false){
            letra = temp.at(0);
            if (letra == '>'){
                encontrado=true;
                cout << "Encontrado " <<endl;
            }
         } 
        
        if(temp.find(">") != string::npos){

            //Cuando detecta una nueva secuencia que no sea la primera, carga la que se esta manejando y se prepara para la nueva
            if (!entrada.empty() && sec.getAncho() != temp.size() || archivo.eof()){
                sec.setDatos(entrada);
                sec_entrada.push_back(sec);
                entrada.clear();
                sec.setAncho(0);
                Secuencia sec;
            }

            //Borramos el >
            temp.erase(0, 1);
            sec.setDescripcion(temp);

        }else{
            entrada.push_back(temp);
        }


    }

    //Añadimos la ultima secuencia del archivo 
    if(encontrado==true){
        sec.setDatos(entrada);
        sec_entrada.push_back(sec);
        fas.setFasta(sec_entrada);
    }
    
    if(encontrado == false){
            cout << nombre_entrada << " no es una secuencia" << endl;
            return fas;
    }

    archivo.close();

    return fas;

}

void Fasta::guardar(string nombre_salida, Fasta fas){

    vector<Secuencia>::iterator it;
    vector<Secuencia> vector;

    ofstream archivo(nombre_salida);

    // Verificar si el archivo se abrió correctamente

    if (!archivo.is_open()) {
        cerr << "Error guardando en " << nombre_salida << endl;
        return; // Salir de la función si no se puede crear el archivo
    }

    vector = fas.getFasta();
    it = vector.begin();

        for (it = vector.begin(); it != vector.end(); ++it) {

                archivo << ">" <<it->getDescripcion() << endl;
                
                for (int i = 0; i < it->getDatos().size(); ++i) {
                    archivo << it->getDatos()[i] << endl;
                }

        }



    archivo.close(); // Cerrar el archivo después de escribirlo


}