// ENTREGA 0 - PROYECTO DE ESTRUCTURAS DE DATOS

// Eliana Pardo Montenegro
// Julian Africano Preciado
// Esteban Navas Suárez

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

// Funciones implementadas
vector<string> leer_comando();  
void mostrar_ayuda();  
void procesar_comando(const vector<string>& comando);
void mostrar_ayuda_general();
void mostrar_ayuda_comando(const string& comando);


// Estructura para representar un comando y la cantidad de argumentos que necesita
struct Comando {
    string nombre;
    int argumentos;
};


// Lista de comandos y su cantidad de argumentos esperados
Comando comandos_validos[] = {
    {"cargar", 1},
    {"listar_secuencias", 0},
    {"histograma", 1},
    {"es_subsecuencia", 1},
    {"enmascarar", 1},
    {"guardar", 1},
    {"codificar", 1},
    {"decodificar", 1},
    {"ruta_mas_corta", 5},
    {"base_remota", 3},
    {"ayuda", 0}, // Puede ser 0 o 1 - se maneja aparte
    {"salir", 0}
};


// Las entradas del usuario se trabajan mediante vectores para identificar los comandos y los parametros que se pasan al programa
vector<string> leer_comando() {
    string linea;
    do {
        cout << "$ ";
        getline(cin, linea);
    } while (linea.empty()); // Seguir imprimiendo $ con saltos de linea
    
    // La estructura de el vector (y la entrada por consola) es: <nombre_comando> <parametro1> <parametro2> ... <parametron>
    vector<string> comando;

    // Para manejar la cantidad de argumentos utilizamos stringstream, donde podemos separar las entradas a la consola y almacenarlas en el vector.
    stringstream tokenizador(linea);
    string palabra;

    while (tokenizador >> palabra) {
        // El tamaño del vector comando indica el numero de parametros, por ejemplo si un comando necesita un parametro nombre_archivo el vector seria de tamaño 2, donde la primera posicion
        // es el comando y la segunda el parametro nombre_archivo
        comando.push_back(palabra);
    }
    return comando;
}


// Imprime la ayuda general desde un txt para mejores practicas
// Muestra solo la lista de comandos
void mostrar_ayuda_general() {
    ifstream archivo("comandos.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir comandos.txt" << endl;
        return;
    }
    string linea;
    while (getline(archivo, linea)) {
        if (linea == "Descripciones") {
            break; // detener antes de las descripciones
        }
        cout << linea << endl;
    }
    archivo.close();
}

// Muestra la descripcion de un comando especifico
void mostrar_ayuda_comando(const string& comando) {
    ifstream archivo("comandos.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir comandos.txt" << endl;
        return;
    }
    string linea;
    bool mostrar = false;
    while (getline(archivo, linea)) {
        if (linea == "[" + comando + "]") {
            mostrar = true;
            cout << linea << endl;
            continue;
        }
        if (mostrar) {
            if (!linea.empty() && linea.front() == '[') { 
                break; // Para cuando llega a otro comando
            }
            cout << linea << endl;
        }
    }
    archivo.close();
}


// Procesa y valida los comandos ingresados usando el arreglo
void procesar_comando(const vector<string>& comando) {
    if (comando.empty()) return;
    
    string nombre = comando[0];

    // Caso especial: salir
    if (nombre == "salir") {
        exit(0);
    }

    // Caso especial: ayuda
    if (nombre == "ayuda") {
        if (comando.size() == 1) {
            mostrar_ayuda_general();
        } else if (comando.size() == 2) {
            // validar que el comando existe antes de mostrar ayuda
            bool existe = false;
            for (int i = 0; i < 12; i++) { // 12 por la cantidad de comandos
                if (comandos_validos[i].nombre == comando[1]) {
                    existe = true;
                    break;
                }
            }
            if (existe) {
                mostrar_ayuda_comando(comando[1]);
            } else {
                cout << "Error: el comando '" << comando[1] << "' no existe" << endl;
            }
        } else {
            cout << "Error: demasiados argumentos para el comando ayuda" << endl;
        }
        return;
    }

    // Buscar el comando en el arreglo (Caso general)
    bool encontrado = false;
    int esperados = 0;
    for (int i = 0; i < 12; i++) {  // 12 porque es la cantidad de elementos del arreglo (comandos)
        if (comandos_validos[i].nombre == nombre) {
            encontrado = true;
            esperados = comandos_validos[i].argumentos;
            break;
        }
    }

    if (!encontrado) {
        cout << "Error: El comando no existe" << endl;
        return;
    }


    // Verificar cantidad de argumentos
    int recibidos = comando.size() - 1;
    if (recibidos < esperados) {
        cout << "Error: Faltan argumentos para el comando " << nombre << endl;
    } else if (recibidos > esperados && nombre != "ayuda") {
        cout << "Error: Demasiados argumentos para el comando " << nombre << endl;
    } else {
        cout << "Comando '" << nombre << "' reconocido y valido (no implementado aun)" << endl;
    }
}


int main() {
    while (true) {
        vector<string> comando = leer_comando();
        procesar_comando(comando);
    }
    return 0; //Practicas de operativos jajas
}
