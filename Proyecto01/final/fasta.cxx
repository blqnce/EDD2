#include "fasta.h"
#include <fstream>
#include <iostream>

using namespace std;

const vector<Secuencia>& Fasta::getSecuencias() const {
    return this->secuencias;
}

bool Fasta::esta_cargado() const {
    return !this->secuencias.empty();
}

//COMANDOS GENERALES

int Fasta::cargar(const string& nombre_archivo) {
    ifstream archivo(nombre_archivo);
    if (!archivo.is_open()) {
        return -1;
    }

    this->secuencias.clear();
    vector<Secuencia> sec_temp;
    Secuencia sec_actual;
    vector<string> datos_actuales;
    string linea;

    while (getline(archivo, linea)) {
        if (!linea.empty() && linea[0] == '>') {
            if (!sec_actual.getDescripcion().empty()) {
                sec_actual.setDatos(datos_actuales);
                if (!datos_actuales.empty()) sec_actual.setAncho((int)datos_actuales[0].size());
                sec_temp.push_back(sec_actual);
                datos_actuales.clear();
            }
            sec_actual = Secuencia();
            sec_actual.setDescripcion(linea.substr(1));
        } else if (!linea.empty()) {
            datos_actuales.push_back(linea);
        }
    }

    if (!sec_actual.getDescripcion().empty()) {
        sec_actual.setDatos(datos_actuales);
        if (!datos_actuales.empty()) sec_actual.setAncho((int)datos_actuales[0].size());
        sec_temp.push_back(sec_actual);
    }
    
    this->secuencias = sec_temp;
    return this->secuencias.size();
}

//--------------//--------------//--------------//--------------//--------------//--------------//--------------//

bool Fasta::guardar(const string& nombre_archivo) const {
    ofstream archivo(nombre_archivo);
    if (!archivo.is_open()) {
        return false;
    }

    for (vector<Secuencia>::const_iterator it = this->secuencias.begin(); it != this->secuencias.end(); ++it) {
        const Secuencia& s = *it;
        archivo << ">" << s.getDescripcion() << "\n";
        
        vector<string> datos = s.getDatos();
        for (vector<string>::const_iterator d_it = datos.begin(); d_it != datos.end(); ++d_it) {
            archivo << *d_it << "\n";
        }
    }
    return true;
}

//--------------//--------------//--------------//--------------//--------------//--------------//--------------//

int Fasta::contar_subsecuencia(const string& subsecuencia) const {
    if (subsecuencia.empty()) return 0;

    int repeticiones = 0;
    for (vector<Secuencia>::const_iterator it = this->secuencias.begin(); it != this->secuencias.end(); ++it) {
        const Secuencia& sec = *it;
        string concat;
        
        vector<string> datos = sec.getDatos();
        for (vector<string>::const_iterator d_it = datos.begin(); d_it != datos.end(); ++d_it) {
            concat += *d_it;
        }

        size_t pos = 0;
        while ((pos = concat.find(subsecuencia, pos)) != string::npos) {
            repeticiones++;
            pos++;
        }
    }
    return repeticiones;
}

//--------------//--------------//--------------//--------------//--------------//--------------//--------------//

int Fasta::enmascarar(const string& subsecuencia) {
    if (subsecuencia.empty()) return 0;
    
    int mascaras = 0;
    string mascara(subsecuencia.length(), 'X');

    for (vector<Secuencia>::iterator it = this->secuencias.begin(); it != this->secuencias.end(); ++it) {
        Secuencia& sec = *it;
        string concat;
        
        vector<string> datos = sec.getDatos();
        for (vector<string>::const_iterator d_it = datos.begin(); d_it != datos.end(); ++d_it) {
            concat += *d_it;
        }

        size_t pos = 0;
        while ((pos = concat.find(subsecuencia, pos)) != string::npos) {
            concat.replace(pos, subsecuencia.length(), mascara);
            mascaras++;
            pos += subsecuencia.length();
        }

        vector<string> nuevos_datos;
        int ancho = sec.getAncho();
        if(ancho == 0) continue;

        for (size_t i = 0; i < concat.length(); i += ancho) {
            nuevos_datos.push_back(concat.substr(i, ancho));
        }
        sec.setDatos(nuevos_datos);
    }
    return mascaras;
}

//--------------//--------------//--------------//--------------//--------------//--------------//--------------//

bool Fasta::histograma(const string& desc_secuencia) const {
    const Secuencia* sec_encontrada = NULL;
    for (vector<Secuencia>::const_iterator it = this->secuencias.begin(); it != this->secuencias.end(); ++it) {
        if (it->getDescripcion() == desc_secuencia) {
            sec_encontrada = &(*it);
            break;
        }
    }

    if (sec_encontrada == NULL) {
        return false;
    }

    int conteo[256] = {0};

    vector<string> datos = sec_encontrada->getDatos();
    for (vector<string>::const_iterator it = datos.begin(); it != datos.end(); ++it) {
        const string& linea = *it;
        for (size_t i = 0; i < linea.length(); ++i) {
            unsigned char base = linea[i]; 
            conteo[base]++;
        }
    }
    
    const vector<char> orden = {
        'A', 'C', 'G', 'T', 'U', 'R', 'Y', 'K', 'M', 'S',
        'W', 'B', 'D', 'H', 'V', 'N', 'X', '-'
    };

    for (vector<char>::const_iterator it = orden.begin(); it != orden.end(); ++it) {
        char codigo = *it;
        if (conteo[codigo] > 0) {
            cout << codigo << " " << conteo[codigo] << endl;
        }
    }
    return true;
}