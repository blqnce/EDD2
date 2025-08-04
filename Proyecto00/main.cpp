#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

//Funcion para la implementacion de la consola
vector<string> entrada();

int main(int argc, char *argv[]) { 
  //Se realiza un bucle "permanente" en el sistema para que la unica forma de salir del programa sea con la indicacion del usuario.
  while (true) {
    vector<string> comando = entrada();
    if (comando[0] == "salir") {
            exit(0);
    }
    else if (comando[0] == "cargar" && comando.size() == 2){
      
    }
    else if (comando[0] == "listar_secuencias" && comando.size() == 1){
      
    }
    else if (comando[0] == "ayuda" && comando.size() == 1) {
            cout << "\n========== LISTA DE COMANDOS ==========" << endl;
            cout << "- cargar <nombre_archivo>" << endl;
            cout << "- listar_secuencias" << endl;
            cout << "- salir" << endl;
            cout << "=======================================" << endl;
            cout << "Use 'ayuda <comando>' para obtener información detallada." << endl;
    }
    else if (comando[0] == "ayuda" && comando.size() > 1) {
            if (comando[1] == "cargar")
                cout << "(cargar <nombre_archivo>)\nCarga un archivo en el sistema." << endl;
            else if (comando[1] == "listar_secuencias")
                cout << "(listar_secuencias)\nImprime la cantidad de secuencias en memoria y el contenido de estas." << endl;
            else if (comando[1] == "salir")
                cout << "(salir)\nTermina la ejecucion del programa." << endl;
            else
                cout << "Error: Comando inexistente.\n";
    }
    else {
            cout << "\nError: Comando inválido.\n";
    }
      
  }
  
}

//Las entradas del usuario se trabajan mediante vectores para identificar las entradas y los parametros que se pasan al programa.

vector<string> entrada() {
    string in;
    do{
      cout << "$ ";
      getline(cin, in);} 
    while (in.empty());
    // Para manejar la cantidad de argumentos utilizamos stringstream, donde podemos separar las entradas a la consola y almacenarlas en el vector.
    stringstream ss(in);
    // La estructura de el vector (y la entrada por consola) es: <nombre_comando> <parametro1> <parametro2> ... <parametron>
    vector<string> comando;
    while (ss >> in) {
        // El tamaño del vector comando indica el numero de parametros, por ejemplo si un comando necesita un parametro nombre_archivo el vector seria de tamaño 2, donde la primera posicion
        // es el comando y la segunda posicion el parametro nombre_archivo
        comando.push_back(in);
    }
    return comando;
}
