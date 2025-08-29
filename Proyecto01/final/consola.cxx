#include "consola.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct Comando {
    string nombre;
    int argumentos;
};

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

void Consola::iniciar() {
    while (true) {
        vector<string> comando = leer_comando();
        if (comando.empty()) {
            continue;
        }
        if (comando[0] == "salir") {
            break;
        }
        procesar_comando(comando);
    }
}

vector<string> Consola::leer_comando() {
    string linea;
    cout << "$ ";
    getline(cin, linea);
    
    vector<string> comando;
    stringstream tokenizador(linea);
    string palabra;
    while (tokenizador >> palabra) {
        comando.push_back(palabra);
    }
    return comando;
}

void Consola::mostrar_ayuda_general() {
    ifstream archivo("comandos.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir comandos.txt" << endl;
        return;
    }
    string linea;
    while (getline(archivo, linea)) {
        if (linea == "Descripciones") {
            break;
        }
        cout << linea << endl;
    }
    archivo.close();
}

void Consola::mostrar_ayuda_comando(const string& comando) {
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
                break;
            }
            cout << linea << endl;
        }
    }
    archivo.close();
}

void Consola::procesar_comando(const vector<string>& comando) {
    string nombre = comando[0];
    int recibidos = comando.size() - 1;

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
    int num_comandos_validos = sizeof(comandos_validos) / sizeof(Comando);
    for (int i = 0; i < num_comandos_validos; ++i) {
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

    if (recibidos < esperados) {
        cout << "Error: Faltan argumentos para el comando " << nombre << endl;
        return;
    }
    if (recibidos > esperados) {
        cout << "Error: Demasiados argumentos para el comando " << nombre << endl;
        return;
    }

    if (nombre == "cargar") {
        int res = fasta_data.cargar(comando[1]);
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
        if (!fasta_data.histograma(comando[1])) {
            cout << "Secuencia invalida." << endl;
        }
    } else if (nombre == "es_subsecuencia") {
        if (!fasta_data.esta_cargado()) { cout << "No hay secuencias cargadas en memoria" << endl; return; }
        int conteo = fasta_data.contar_subsecuencia(comando[1]);
        if (conteo == 0) cout << "La subsecuencia dada no existe dentro de las secuencias cargadas en memoria" << endl;
        else cout << "La subsecuencia dada se repite " << conteo << " veces dentro de las secuencias cargadas en memoria" << endl;

    } else if (nombre == "enmascarar") {
        if (!fasta_data.esta_cargado()) { cout << "No hay secuencias cargadas en memoria." << endl; return; }
        int conteo = fasta_data.enmascarar(comando[1]);
        if (conteo == 0) cout << "La subsecuencia dada no existe dentro de las secuencias cargadas en memoria, por tanto no se enmascara nada" << endl;
        else cout << conteo << " subsecuencias han sido enmascaradas dentro de las secuencias cargadas en memoria" << endl;

    } else if (nombre == "guardar") {
        if (!fasta_data.esta_cargado()) { cout << "No hay secuencias cargadas en memoria" << endl; return; }
        if (fasta_data.guardar(comando[1])) cout << "Las secuencias han sido guardadas en " << comando[1] << endl;
        else cout << "Error guardando en " << comando[1] << endl;
    }
}