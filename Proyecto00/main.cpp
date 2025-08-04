#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

//Funcion para la implementacion de la consola
vector<string> userIn();

int main(int argc, char *argv[]) { 
  //Se realiza un bucle "permanente" en el sistema para que la unica forma de salir del programa sea con salir.
  while (true) {
    vector<string> comando = userIn();
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
            cout << "- salir\" << endl;
            cout << "=======================================" << endl;
            cout << "Use 'ayuda <comando>' para obtener información detallada." << endl;
    }
    else if (comando[0] == "ayuda" && comando.size() > 1) {
            if (comando[1] == "cargar")
                cout << "\n(cargar <nombre_archivo>)\nCarga un archivo en el sistema." << endl;
            else if (comando[1] == "listar_secuencias")
                cout << "\n(listar_secuencias)\nImprime la cantidad de secuencias en memoria y el contenido de estas." << endl;
            else if (comando[1] == "salir")
                cout << "\n(salir)\nTermina la ejecucion del programa." << endl;
            else
                cout << "\nError: Comando inexistente.\n";
    }
    else {
            cout << "\nError: Comando inválido.\n";
    }
      
  }
  
}

//Las entradas del usuario se trabajan mediante vectores para identificar los inputs y los parametros que se pasan al programa.

vector<string> userIn() {
    string in;
    do{
      cout << "$ ";
      getline(cin, in);} 
    while (in.empty());
  
    stringstream ss(in);
    vector<string> comando;
    while (ss >> in) {
        comando.push_back(in);
    }
    return comando;
}
