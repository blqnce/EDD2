// Comando general
// g++ secuencia.cxx fasta.cxx arbol_huffman.cxx grafo.cxx consola.cxx main.cxx -o prueba

#include "fasta.h"
#include "arbol_huffman.h"
#include "grafo.h"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <bitset> 

using namespace std;

const vector<Secuencia>& Fasta::getSecuencias() const {
    return this->secuencias;
}

bool Fasta::esta_cargado() const {
    return !this->secuencias.empty();
}

//COMANDOS GENERALES

//--------------//--------------//--------------//--------------//--------------//--------------//--------------//
// cargar <nombre_archivo>

int Fasta::cargar(const string& nombre_archivo) {

    //Apertura del archivo en modo lectura
    ifstream archivo(nombre_archivo);
    //Verificamos la apertura del archivo, en caso de que ocurra un error retornamos el codigo de error -1
    if (!archivo.is_open()) {
        return -1;
    }


    // Limpiamos la estructura Fasta actual en memoria para cargar la entrante
    this->secuencias.clear();

    // Declaracion variables temporales para manejar la entrada de datos
    vector<Secuencia> sec_temp;
    Secuencia sec_actual;
    vector<string> datos_actuales;
    string linea;


    // Ciclo para leer el contenido del archivo mientras tenga lineas no vacias
    while (getline(archivo, linea)) {
        
        if (!linea.empty() && linea[0] == '>') { // Comprobacion de si la linea actual es una descripcion de una secuencia
            if (!sec_actual.getDescripcion().empty()) { // Si la secuencia no tiene una descripcion significa que es una nueva secuencia, en ese caso se agrega al vector de secuencias la secuancia actual para dejar espacio a la nueva
                sec_actual.setDatos(datos_actuales);
                // Si el vector de string no esta vacio, significa que ya se leyeron todos los datos de la secuencia y se añaden junto a el ancho que es dado por el primer dato del vector
                if (!datos_actuales.empty()) sec_actual.setAncho((int)datos_actuales[0].size()); 
                sec_temp.push_back(sec_actual);
                datos_actuales.clear(); // Limpiamos nuestro vector de datos
            }
            sec_actual = Secuencia();
            // Establecemos la descripcion de la secuencia
            sec_actual.setDescripcion(linea.substr(1));
        } else if (!linea.empty()) { // En caso de que la linea no sea una descripcion se añade al vector de string actual como datos
            datos_actuales.push_back(linea);
        }
    }
    // Añadimos al vector la ultima secuencia encontrada, tambien es el que agrega la secuencia si en el archivo solo existe una
    if (!sec_actual.getDescripcion().empty()) {
        sec_actual.setDatos(datos_actuales);
        if (!datos_actuales.empty()) sec_actual.setAncho((int)datos_actuales[0].size());
        sec_temp.push_back(sec_actual);
    }
    // Establecemos la estructura fasta global del programa con el contenido del vector de secuencias temporal
    this->secuencias = sec_temp;
    // Retornamos el tamaño del vector para el conteo de cuantas secuencias tiene
    return this->secuencias.size();
}

//--------------//--------------//--------------//--------------//--------------//--------------//--------------//
// guardar <nombre_archivo>

bool Fasta::guardar(const string& nombre_archivo) const {
    //Creacion del archivo en modo escritura
    ofstream archivo(nombre_archivo);
    //Verificamos la apertura del archivo, en caso de que ocurra algun error retornamos falso para el manejo de errores
    if (!archivo.is_open()) {
        return false;
    }

    //Iteramos la estructura fasta en memoria 
    for (vector<Secuencia>::const_iterator it = this->secuencias.begin(); it != this->secuencias.end(); ++it) {
        // Por cada descripcion le añadimos el simbolo ">" que eliminamos previamente para almacenarlo en memoria y lo escribimos
        const Secuencia& s = *it;
        archivo << ">" << s.getDescripcion() << "\n";
        // Escribimos todos los datos pertenecientes a esa secuencia
        vector<string> datos = s.getDatos();
        for (vector<string>::const_iterator d_it = datos.begin(); d_it != datos.end(); ++d_it) {
            archivo << *d_it << "\n";
        }
    }
    // Retornamos verdadero para indicar que se pudo escribir el contenido de la estructura fasta en el archivo
    return true;
}

//--------------//--------------//--------------//--------------//--------------//--------------//--------------//
// listar_secuencias
void Fasta::listar_secuencias() const {
    if (this->secuencias.empty()) {
        cout << "No hay secuencias cargadas en memoria." << endl;
        return;
    }

    cout << "Hay " << this->secuencias.size() << " secuencias cargadas en memoria:" << endl;

    for (vector<Secuencia>::const_iterator it = this->secuencias.begin(); it != this->secuencias.end(); ++it) {
        const Secuencia& s = *it;
        size_t total_bases = 0;
        bool incompleta = false;
        
        // Obtenemos los datos para analizarlos
        vector<string> datos = s.getDatos();
        for (vector<string>::const_iterator d_it = datos.begin(); d_it != datos.end(); ++d_it) {
            const string& linea = *d_it;
            for (size_t i = 0; i < linea.length(); ++i) {
                char c = linea[i];
                if (c == '-') {
                    incompleta = true;
                } else {
                    // Contamos solo si no es un guion, o contamos todo dependiendo de tu logica anterior.
                    // En tu codigo original contabas todo en total_bases y luego bases_reales aparte.
                    // Simplificando segun tu logica original:
                    total_bases++; 
                }
            }
        }

        // Si es incompleta, debemos recalcular o ajustar el mensaje segun el enunciado
        if (incompleta) {
            size_t bases_reales = 0;
            for (vector<string>::const_iterator d_it = datos.begin(); d_it != datos.end(); ++d_it) {
                const string& linea = *d_it;
                for (size_t i = 0; i < linea.length(); ++i) {
                    if (linea[i] != '-') bases_reales++;
                }
            }
            cout << "Secuencia " << s.getDescripcion() << " contiene al menos " << bases_reales << " bases." << endl;
        } else {
            cout << "Secuencia " << s.getDescripcion() << " contiene " << total_bases << " bases." << endl;
        }
    }
}

//--------------//--------------//--------------//--------------//--------------//--------------//--------------//
// es_subsecuencia <subsecuencia>

int Fasta::contar_subsecuencia(const string& subsecuencia) const {
    // Si la subsecuencia a buscar esta vacia no hay nada que hacer
    if (subsecuencia.empty()) return 0;

    // Se inicializa un contador para las repeticiones
    int repeticiones = 0;
    // Se itera sobre cada secuencia que este cargada en memoria
    for (vector<Secuencia>::const_iterator it = this->secuencias.begin(); it != this->secuencias.end(); ++it) {
        // Se obtiene una referencia constante a la secuencia actual
        const Secuencia& sec = *it;
        // Se crea un string para unir todos los datos de la secuencia
        string concat;
        
        // Se obtienen los datos de la secuencia que estan en un vector de strings
        vector<string> datos = sec.getDatos();
        // Se recorre el vector de datos y se concatena cada linea en el string 'concat'
        for (vector<string>::const_iterator d_it = datos.begin(); d_it != datos.end(); ++d_it) {
            concat += *d_it;
        }

        // Se inicializa la posicion de busqueda en 0
        size_t pos = 0;
        // Se busca la subsecuencia dentro del string concatenado
        // El bucle continua mientras 'find' encuentre una ocurrencia
        while ((pos = concat.find(subsecuencia, pos)) != string::npos) {
            // Si se encuentra se incrementa el contador
            repeticiones++;
            // Se avanza la posicion de busqueda en 1 para encontrar subsecuencias solapadas
            pos++;
        }
    }
    // Se devuelve el total de repeticiones encontradas
    return repeticiones;
}

//--------------//--------------//--------------//--------------//--------------//--------------//--------------//
// enmascarar <subsecuencia>

int Fasta::enmascarar(const string& subsecuencia) {
    // Si la subsecuencia a enmascarar esta vacia no se hace nada
    if (subsecuencia.empty()) return 0;
    
    // Se inicializa un contador para las mascaras aplicadas
    int mascaras = 0;
    // Se crea un string de 'X' con la misma longitud que la subsecuencia
    string mascara(subsecuencia.length(), 'X');

    // Se itera sobre cada secuencia para poder modificarla
    for (vector<Secuencia>::iterator it = this->secuencias.begin(); it != this->secuencias.end(); ++it) {
        // Se obtiene una referencia a la secuencia actual para poder cambiar sus datos
        Secuencia& sec = *it;
        // Se crea un string para unir todos los datos de la secuencia
        string concat;
        
        // Se obtienen los datos de la secuencia
        vector<string> datos = sec.getDatos();
        // Se concatenan los datos en un solo string
        for (vector<string>::const_iterator d_it = datos.begin(); d_it != datos.end(); ++d_it) {
            concat += *d_it;
        }

        // Se inicializa la posicion de busqueda en 0
        size_t pos = 0;
        // Se busca la subsecuencia dentro del string concatenado
        while ((pos = concat.find(subsecuencia, pos)) != string::npos) {
            // Si se encuentra se reemplaza por la mascara de 'X'
            concat.replace(pos, subsecuencia.length(), mascara);
            // Se incrementa el contador de mascaras
            mascaras++;
            // Se avanza la posicion por el largo de la palabra para no enmascarar lo ya enmascarado
            pos += subsecuencia.length();
        }

        // Se crea un nuevo vector para guardar los datos modificados
        vector<string> nuevos_datos;
        // Se obtiene el ancho original de las lineas de la secuencia
        int ancho = sec.getAncho();
        // Si el ancho es 0 se salta a la siguiente secuencia para evitar errores
        if(ancho == 0) continue;

        // Se reconstruye el vector de datos a partir del string modificado
        // cortandolo en pedazos del tamaño del ancho original
        for (size_t i = 0; i < concat.length(); i += ancho) {
            nuevos_datos.push_back(concat.substr(i, ancho));
        }
        // Se actualizan los datos de la secuencia con el nuevo vector enmascarado
        sec.setDatos(nuevos_datos);
    }
    // Se devuelve el total de mascaras aplicadas
    return mascaras;
}

//--------------//--------------//--------------//--------------//--------------//--------------//--------------//
// histograma <descripcion>

bool Fasta::histograma(const string& desc_secuencia) const {
    // Se crea un puntero para almacenar la secuencia que se encuentre
    const Secuencia* sec_encontrada = NULL;
    // Se itera sobre todas las secuencias para buscar la que coincida con la descripcion
    for (vector<Secuencia>::const_iterator it = this->secuencias.begin(); it != this->secuencias.end(); ++it) {
        if (it->getDescripcion() == desc_secuencia) {
            // Si se encuentra se guarda su direccion en el puntero y se rompe el bucle
            sec_encontrada = &(*it);
            break;
        }
    }

    // Si no se encontro ninguna secuencia (el puntero sigue siendo NULL) se retorna falso
    if (sec_encontrada == NULL) {
        return false;
    }

    // Se crea un arreglo de 256 enteros para contar la frecuencia de cada caracter ASCII
    int conteo[256] = {0};

    // Se obtienen los datos de la secuencia encontrada
    vector<string> datos = sec_encontrada->getDatos();
    // Se itera sobre cada linea de datos
    for (vector<string>::const_iterator it = datos.begin(); it != datos.end(); ++it) {
        const string& linea = *it;
        // Se itera sobre cada caracter de la linea
        for (size_t i = 0; i < linea.length(); ++i) {
            // Se usa el valor ASCII del caracter como indice para el arreglo de conteo
            char base = linea[i]; 
            conteo[base]++;
        }
    }
    
    // Se define un vector con el orden especifico para imprimir el histograma
    const vector<char> orden = {
        'A', 'C', 'G', 'T', 'U', 'R', 'Y', 'K', 'M', 'S',
        'W', 'B', 'D', 'H', 'V', 'N', 'X', '-'
    };

    // Se itera sobre el vector 'orden' para imprimir los resultados en ese orden
    for (vector<char>::const_iterator it = orden.begin(); it != orden.end(); ++it) {
        char codigo = *it;
        // Si el conteo para un caracter es mayor a 0 se imprime
        if (conteo[codigo] > 0) {
            cout << codigo << " " << conteo[codigo] << endl;
        }
    }
    // Se retorna verdadero para indicar que el histograma se mostro correctamente
    return true;
}

//--------------//--------------//--------------//--------------//--------------//--------------//--------------//
// codificar <nombre_archivo.fabin>

bool Fasta::codificar(const string& nombre_archivo) {
    // 1 Contar frecuencias de todos los caracteres
    unordered_map<char, int> frecuencias;
    string datosCompletos = "";

    for (int i = 0; i < this->secuencias.size(); ++i) {
        const Secuencia& sec = this->secuencias[i];
        vector<string> datos = sec.getDatos();
        for (int j = 0; j < datos.size(); ++j) {
            datosCompletos += datos[j];
        }
    }
    for (int i = 0; i < datosCompletos.length(); ++i) {
        frecuencias[datosCompletos[i]]++;
    }

    if (frecuencias.empty()) return false;

    // 2 Crear el arbol y la tabla de codigos
    ArbolHuffman arbol(frecuencias);
    unordered_map<char, string> tablaCodigos = arbol.generarTablaCodigos();

    // 3 Escribir en el archivo binario
    ofstream archivoSalida(nombre_archivo, ios::binary);
    if (!archivoSalida.is_open()) return false;

    // Escribimos la cantidad de caracteres unicos
    int numBasesUnicas = frecuencias.size();
    archivoSalida.write(reinterpret_cast<const char*>(&numBasesUnicas), sizeof(int));

    // Escribimos la tabla de frecuencias
    for (const pair<const char, int>& par : frecuencias) {
        archivoSalida.write(&par.first, sizeof(char));
        int freq = par.second;
        archivoSalida.write(reinterpret_cast<const char*>(&freq), sizeof(int));
    }

    // Escribimos los datos de las secuencias
    int numSecuencias = this->secuencias.size();
    archivoSalida.write(reinterpret_cast<const char*>(&numSecuencias), sizeof(int));
    
    for (int i = 0; i < this->secuencias.size(); ++i) {
        const Secuencia& sec = this->secuencias[i];
        
        int tamNombre = sec.getDescripcion().length();
        archivoSalida.write(reinterpret_cast<const char*>(&tamNombre), sizeof(int));
        archivoSalida.write(sec.getDescripcion().c_str(), tamNombre);
        
        int longitudSecuencia = 0;
        vector<string> datos = sec.getDatos();
        for (int j = 0; j < datos.size(); ++j) {
            longitudSecuencia += datos[j].length();
        }
        archivoSalida.write(reinterpret_cast<const char*>(&longitudSecuencia), sizeof(int));

        int ancho = sec.getAncho();
        archivoSalida.write(reinterpret_cast<const char*>(&ancho), sizeof(int));
    }
    
    // Escribimos los datos comprimidos
    string bufferBits = "";
    for (int i = 0; i < datosCompletos.length(); ++i) {
        bufferBits += tablaCodigos[datosCompletos[i]];
    }

    for (size_t i = 0; i < bufferBits.length(); i += 8) {
        string byteString = bufferBits.substr(i, 8);
        bitset<8> byte(byteString);
        char valorByte = static_cast<char>(byte.to_ulong());
        archivoSalida.write(&valorByte, sizeof(char));
    }

    archivoSalida.close();
    return true;
}

//--------------//--------------//--------------//--------------//--------------//--------------//--------------//
// decodificar <nombre_archivo.fabin>

bool Fasta::decodificar(const string& nombre_archivo) {
    ifstream archivoEntrada(nombre_archivo, ios::binary);
    if (!archivoEntrada.is_open()) return false;

    this->secuencias.clear();

    // Leemos el encabezado del archivo
    int numBasesUnicas;
    archivoEntrada.read(reinterpret_cast<char*>(&numBasesUnicas), sizeof(int));

    unordered_map<char, int> frecuencias;
    for (int i = 0; i < numBasesUnicas; ++i) {
        char base;
        int freq;
        archivoEntrada.read(&base, sizeof(char));
        archivoEntrada.read(reinterpret_cast<char*>(&freq), sizeof(int));
        frecuencias[base] = freq;
    }

    // Reconstruimos el arbol
    ArbolHuffman arbol(frecuencias);

    // Leemos los metadatos de las secuencias
    int numSecuencias;
    archivoEntrada.read(reinterpret_cast<char*>(&numSecuencias), sizeof(int));

    int longitudTotalDatos = 0;
    vector<Secuencia> secuenciasTemporales;

    for(int i=0; i < numSecuencias; ++i) {
        Secuencia sec;
        int tamNombre;
        archivoEntrada.read(reinterpret_cast<char*>(&tamNombre), sizeof(int));
        
        string descripcion(tamNombre, '\0');
        archivoEntrada.read(&descripcion[0], tamNombre);
        sec.setDescripcion(descripcion);

        int longitudSecuencia;
        archivoEntrada.read(reinterpret_cast<char*>(&longitudSecuencia), sizeof(int));
        longitudTotalDatos += longitudSecuencia;

        int ancho;
        archivoEntrada.read(reinterpret_cast<char*>(&ancho), sizeof(int));
        sec.setAncho(ancho);

        secuenciasTemporales.push_back(sec);
    }

    // Leemos los datos comprimidos
    string bufferBits = "";
    char byte;
    while(archivoEntrada.read(&byte, sizeof(char))){
        bufferBits += bitset<8>(static_cast<char>(byte)).to_string();
    }
    
    // Decodificamos y reconstruimos las secuencias
    string datosDecodificados = arbol.decodificar(bufferBits, longitudTotalDatos);

    size_t posActual = 0;
    for (int i = 0; i < secuenciasTemporales.size(); ++i) {
        // Volvemos a leer la longitud de cada secuencia del archivo para saber cuanto cortar
        int longitudSecuencia = 0;
        ifstream temp(nombre_archivo, ios::binary);
        long pos_a_buscar = sizeof(int) + numBasesUnicas * (sizeof(char) + sizeof(int)) + sizeof(int);
        for(int k = 0; k < i; ++k){
            // Para saltar a la seccion correcta, necesitamos saber el tamaño del nombre de las secuencias anteriores
            int tamNombrePrevio = secuenciasTemporales[k].getDescripcion().length();
            pos_a_buscar += sizeof(int) + tamNombrePrevio + sizeof(int) + sizeof(int);
        }
        temp.seekg(pos_a_buscar, ios::beg);
        int tamNombreTemp;
        temp.read(reinterpret_cast<char*>(&tamNombreTemp), sizeof(int));
        string descTemp(tamNombreTemp, '\0');
        temp.read(&descTemp[0], tamNombreTemp);
        temp.read(reinterpret_cast<char*>(&longitudSecuencia), sizeof(int));
        temp.close();


        string datosDeEstaSec = datosDecodificados.substr(posActual, longitudSecuencia);
        posActual += longitudSecuencia;

        int ancho = secuenciasTemporales[i].getAncho();
        vector<string> datosFormateados;
        for (size_t j = 0; j < datosDeEstaSec.length(); j += ancho) {
            datosFormateados.push_back(datosDeEstaSec.substr(j, ancho));
        }
        secuenciasTemporales[i].setDatos(datosFormateados);
    }
    
    this->secuencias = secuenciasTemporales;

    archivoEntrada.close();
    return true;
}

//--------------//--------------//--------------//--------------//--------------//--------------//--------------//
// ruta_mas_corta <descripcion_secuencia> <i> <j> <x> <y>

bool Fasta::ruta_mas_corta(const string& desc_secuencia, int i, int j, int x, int y) const {
    // 1. Buscar Secuencia
    const Secuencia* sec = nullptr;
    for (vector<Secuencia>::const_iterator it = secuencias.begin(); it != secuencias.end(); ++it) {
        if (it->getDescripcion() == desc_secuencia) {
            sec = &(*it);
            break;
        }
    }

    if (sec == nullptr) {
        cout << "(la secuencia no existe) La secuencia " << desc_secuencia << " no existe." << endl;
        return false;
    }

    // 2. Instanciar Grafo y Consultar
    Grafo grafo(sec->getDatos(), sec->getAncho());
    
    // Validacion previa de coordenadas para mensaje especifico del enunciado
    // Usamos el metodo auxiliar del grafo para saber si la coordenada existe sin calcular rutas aun
    if (grafo.obtener_base(i, j) == '\0') {
         cout << "(posicion de base origen invalida) La base en la posicion [" << i << "," << j << "] no existe." << endl;
         return false;
    }
    if (grafo.obtener_base(x, y) == '\0') {
         cout << "(posicion de base destino invalida) La base en la posicion [" << x << "," << y << "] no existe." << endl;
         return false;
    }

    ResultadoCamino res = grafo.calcular_ruta(i, j, x, y);

    // 3. Imprimir Resultado segun formato del enunciado
    cout << "(la secuencia existe) Para la secuencia " << desc_secuencia << ", la ruta mas corta entre ";
    cout << "la base " << grafo.obtener_base(i, j) << " en [" << i << "," << j << "] y ";
    cout << "la base " << grafo.obtener_base(x, y) << " en [" << x << "," << y << "] es:" << endl;

    if (!res.existe) {
        cout << "No existe camino alcanzable." << endl;
    } else {
        for (size_t k = 0; k < res.ruta.size(); ++k) {
            cout << res.ruta[k];
            if (k < res.ruta.size() - 1) cout << " -> ";
        }
        cout << endl;
        cout << "El costo total de la ruta es: " << res.costo_total << endl;
    }
    return true;
}

//--------------//--------------//--------------//--------------//--------------//--------------//--------------//
// base_remota <descripcion_secuencia> <i> <j>

bool Fasta::base_remota(const string& desc_secuencia, int i, int j) const {
    // 1. Buscar Secuencia
    const Secuencia* sec = nullptr;
    for (vector<Secuencia>::const_iterator it = secuencias.begin(); it != secuencias.end(); ++it) {
        if (it->getDescripcion() == desc_secuencia) {
            sec = &(*it);
            break;
        }
    }
    if (sec == nullptr) {
        cout << "(la secuencia no existe) La secuencia " << desc_secuencia << " no existe." << endl;
        return false;
    }

    // 2. Instanciar Grafo
    Grafo grafo(sec->getDatos(), sec->getAncho());

    if (grafo.obtener_base(i, j) == '\0') {
         cout << "(posicion de base invalida) La base en la posicion [" << i << "," << j << "] no existe." << endl;
         return false;
    }

    // 3. Consultar Grafo
    ResultadoCamino res = grafo.buscar_base_remota(i, j);

    // 4. Imprimir
    if (!res.existe) {
        cout << "No se encontraron otras bases identicas alcanzables." << endl;
    } else {
        // Para sacar la coordenada final debemos parsear el string o devolverlo en el struct
        // El ultimo elemento de res.ruta es el destino
        // Formato [f,c]L
        string ultimo = res.ruta.back();
        size_t cierre = ultimo.find(']');
        string coord = ultimo.substr(0, cierre+1); // [f,c]

        cout << "(la base existe) Para la secuencia " << desc_secuencia << ", la base remota esta ubicada en " << coord << endl;
        cout << "y la ruta entre la base en [" << i << "," << j << "] y la base remota en " << coord << " es:" << endl;
        
        for (size_t k = 0; k < res.ruta.size(); ++k) {
            cout << res.ruta[k];
            if (k < res.ruta.size() - 1) cout << " -> ";
        }
        cout << endl;
        cout << "El costo total de la ruta es: " << res.costo_total << endl;
    }
    return true;
}