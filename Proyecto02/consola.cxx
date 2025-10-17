#include "consola.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Se define una estructura para guardar el nombre de un comando y cuantos argumentos necesita
struct Comando {
    string nombre;
    int argumentos;
};

// Se crea un arreglo estatico con todos los comandos validos y sus numeros de argumentos
// ayuda tiene -1 porque puede tener 0 o 1 argumento y se maneja como caso especial
static Comando comandos_validos[] = {
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
    {"ayuda", -1},
    {"salir", 0}
};

// Metodo que inicia el bucle infinito de la consola
void Consola::iniciar() {
    while (true) {
        // Se lee el comando del usuario
        vector<string> comando = leer_comando();
        if (comando.empty()) {
            // Si el comando esta vacio se continua a la siguiente iteracion
            continue;
        }
        if (comando[0] == "salir") {
            // Si el comando es "salir" se rompe el bucle y termina el programa
            break;
        }
        // Si no es salir ni vacio se procesa el comando
        procesar_comando(comando);
    }
}

// Metodo para leer una linea de la entrada estandar y separarla en palabras
vector<string> Consola::leer_comando() {
    string linea;
    // Se muestra el simbolo de la consola
    cout << "$ ";
    // Se lee la linea completa que introduce el usuario
    getline(cin, linea);
    
    vector<string> comando;
    // Se usa un stringstream para dividir la linea en palabras
    stringstream tokenizador(linea);
    string palabra;
    while (tokenizador >> palabra) {
        // Cada palabra se añade al vector de strings
        comando.push_back(palabra);
    }
    return comando;
}

// Muestra la ayuda general leyendo desde un archivo de texto
void Consola::mostrar_ayuda_general() {
    ifstream archivo("comandos.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir comandos.txt" << endl;
        return;
    }
    string linea;
    while (getline(archivo, linea)) {
        // Se asume un formato especifico en el archivo para detener la lectura
        if (linea == "Descripciones") {
            break;
        }
        cout << linea << endl;
    }
    archivo.close();
}

// Muestra la ayuda para un comando especifico
void Consola::mostrar_ayuda_comando(const string& comando) {
    ifstream archivo("comandos.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir comandos.txt" << endl;
        return;
    }
    string linea;
    bool mostrar = false;
    while (getline(archivo, linea)) {
        // Busca la seccion del comando especifico en el archivo de ayuda
        if (linea == "[" + comando + "]") {
            mostrar = true;
            cout << linea << endl;
            continue;
        }
        if (mostrar) {
            // Si encuentra el inicio de otra seccion de comando se detiene
            if (!linea.empty() && linea.front() == '[') { 
                break;
            }
            cout << linea << endl;
        }
    }
    archivo.close();
}

// Procesa el comando introducido por el usuario
void Consola::procesar_comando(const vector<string>& comando) {
    // La primera palabra es el nombre del comando
    string nombre = comando[0];
    // El resto son los argumentos
    int recibidos = comando.size() - 1;

    // Caso especial para el comando "ayuda"
    if (nombre == "ayuda") {
        if (recibidos == 0) {
            mostrar_ayuda_general();
        } else if (recibidos == 1) {
            mostrar_ayuda_comando(comando[1]);
        } else {
            cout << "Error: demasiados argumentos para el comando ayuda" << endl;
        }
        return;
    }

    bool encontrado = false;
    int esperados = 0;
    // Se calcula el numero de comandos validos para iterar el arreglo
    int num_comandos_validos = sizeof(comandos_validos) / sizeof(Comando);
    for (int i = 0; i < num_comandos_validos; ++i) {
        // Se busca si el comando del usuario existe en nuestra lista
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

    // Se valida que el numero de argumentos sea el correcto
    if (recibidos < esperados) {
        cout << "Error: Faltan argumentos para el comando " << nombre << endl;
        return;
    }
    if (recibidos > esperados) {
        cout << "Error: Demasiados argumentos para el comando " << nombre << endl;
        return;
    }

    // Este es el bloque que decide que accion tomar
    // Cada comando llama a un metodo del objeto fasta_data

    if (nombre == "cargar") {
        int res = fasta_data.cargar(comando[1]);
        // Se interpreta el resultado devuelto por el metodo cargar
        if (res == -1) cout << comando[1] << " no se encuentra o no puede leerse" << endl;
        else if (res == 0) cout << comando[1] << " no contiene ninguna secuencia" << endl;
        else if (res == 1) cout << "1 secuencia cargada correctamente desde " << comando[1] << endl;
        else cout << res << " secuencias cargadas correctamente desde " << comando[1] << endl;

    } else if (nombre == "listar_secuencias") {
        if (!fasta_data.esta_cargado()) { cout << "No hay secuencias cargadas en memoria" << endl; return; }
        
        const vector<Secuencia>& secuencias = fasta_data.getSecuencias();
        cout << "Hay " << secuencias.size() << " secuencias cargadas en memoria:" << endl;

        for(vector<Secuencia>::const_iterator it = secuencias.begin(); it != secuencias.end(); ++it) {
            const Secuencia& s = *it;
            size_t total_bases = 0;
            bool incompleta = false;
            vector<string> datos = s.getDatos();
            
            for(vector<string>::const_iterator d_it = datos.begin(); d_it != datos.end(); ++d_it) {
                const string& linea = *d_it;
                for(size_t i = 0; i < linea.length(); ++i) {
                    char c = linea[i];
                    if (c == '-') incompleta = true;
                    total_bases++;
                }
            }
            // Logica para diferenciar entre secuencias completas e incompletas al listar
            if (incompleta) {
                size_t bases_reales = 0;
                vector<string> datos_reales = s.getDatos();
                for(vector<string>::const_iterator d_it = datos_reales.begin(); d_it != datos_reales.end(); ++d_it){
                    const string& linea = *d_it;
                    for(size_t i = 0; i < linea.length(); ++i) {
                        char c = linea[i];
                        if (c != '-') bases_reales++;
                    }
                }
                cout << "Secuencia " << s.getDescripcion() << " contiene al menos " << bases_reales << " bases" << endl;
            } else {
                cout << "Secuencia " << s.getDescripcion() << " contiene " << total_bases << " bases" << endl;
            }
        }

    } else if (nombre == "histograma") {
        if (!fasta_data.esta_cargado()) { cout << "No hay secuencias cargadas en memoria" << endl; return; }
        // Se llama al metodo histograma y se verifica si la secuencia fue encontrada
        if (!fasta_data.histograma(comando[1])) {
            cout << "Secuencia invalida" << endl;
        }
    } else if (nombre == "es_subsecuencia") {
        if (!fasta_data.esta_cargado()) { cout << "No hay secuencias cargadas en memoria" << endl; return; }
        int conteo = fasta_data.contar_subsecuencia(comando[1]);
        if (conteo == 0) cout << "La subsecuencia dada no existe dentro de las secuencias cargadas en memoria" << endl;
        else cout << "La subsecuencia dada se repite " << conteo << " veces dentro de las secuencias cargadas en memoria" << endl;

    } else if (nombre == "enmascarar") {
        if (!fasta_data.esta_cargado()) { cout << "No hay secuencias cargadas en memoria" << endl; return; }
        int conteo = fasta_data.enmascarar(comando[1]);
        if (conteo == 0) cout << "La subsecuencia dada no existe dentro de las secuencias cargadas en memoria por tanto no se enmascara nada" << endl;
        else cout << conteo << " subsecuencias han sido enmascaradas dentro de las secuencias cargadas en memoria" << endl;

    } else if (nombre == "guardar") {
        if (!fasta_data.esta_cargado()) { cout << "No hay secuencias cargadas en memoria" << endl; return; }
        if (fasta_data.guardar(comando[1])) cout << "Las secuencias han sido guardadas en " << comando[1] << endl;
        else cout << "Error guardando en " << comando[1] << endl;

    } else if (nombre == "codificar") {
        if (!fasta_data.esta_cargado()) { cout << "No hay secuencias cargadas en memoria" << endl; return; }

        // Se toma el nombre base que nos da el usuario
        string nombre_base = comando[1];
        string nombre_final;

        // Buscamos la posicion del ultimo punto en el nombre
        size_t pos_punto = nombre_base.find_last_of(".");

        // Si encontramos un punto, cortamos el nombre justo ahi y añadimos ".fabin"
        if (pos_punto != string::npos) { // string::npos significa que no se encontro el punto
            nombre_final = nombre_base.substr(0, pos_punto) + ".fabin";
        } else {
            // Si no hay punto, simplemente le añadimos ".fabin" al final
            nombre_final = nombre_base + ".fabin";
        }

        if (fasta_data.codificar(nombre_final)) {
            // Le informamos al usuario el nombre del archivo que se creo
            cout << "Secuencias codificadas y almacenadas en " << nombre_final << endl;
        } else {
            cout << "Error guardando las secuencias en " << nombre_final << endl;}
            
    } else if (nombre == "decodificar") {
        string nombre_archivo = comando[1];

        // Primero aseguramos que el nombre del archivo sea lo suficientemente largo para contener la extension
        // Luego extraemos los ultimos 6 caracteres y los comparamos con ".fabin"
        if (nombre_archivo.length() < 6 || nombre_archivo.substr(nombre_archivo.length() - 6) != ".fabin") { // 6 porque es el numero de caracteres de .fabin
            
            cout << "Error: El archivo para decodificar debe tener la extension .fabin" << endl;
            return; 
        }
       
        if (fasta_data.decodificar(nombre_archivo)) {
            cout << "Secuencias decodificadas desde " << nombre_archivo << " y cargadas en memoria" << endl;
        } else {
            cout << "Error cargando las secuencias desde " << nombre_archivo << endl;
        }
    }
}
