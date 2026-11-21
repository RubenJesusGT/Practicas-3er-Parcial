// Ejercicio 5: Integracion archivo + base de datos (en archivo) con manejo de excepciones
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

// Excepciones propias para distinguir el origen del error
class ArchivoError : public runtime_error {
public:
    explicit ArchivoError(const string& msg) : runtime_error(msg) {}
};

class BaseDatosError : public runtime_error {
public:
    explicit BaseDatosError(const string& msg) : runtime_error(msg) {}
};

string leerTexto(const string& msg) {
    cout << msg;
    string s;
    getline(cin, s);
    return s;
}

vector<string> leerTareas(const string& ruta) {
    ifstream archivo(ruta);
    if (!archivo)
        throw ArchivoError("No se encontro el archivo '" + ruta + "'. Verifica el nombre o la ruta.");
    vector<string> tareas;
    string linea;
    while (getline(archivo, linea))
        if (!linea.empty()) tareas.push_back(linea);
    if (tareas.empty())
        throw ArchivoError("El archivo '" + ruta + "' esta vacio.");
    return tareas;
}

void guardarEnBD(const string& rutaBD, const vector<string>& tareas) {
    // Contar los registros que ya existen para continuar el id
    int ultimoId = 0;
    ifstream existente(rutaBD);
    string linea;
    while (getline(existente, linea))
        if (!linea.empty()) ultimoId++;
    existente.close();

    // Si la carpeta no existe, el archivo no se puede abrir: la conexion falla
    ofstream bd(rutaBD, ios::app);
    if (!bd)
        throw BaseDatosError("No se pudo conectar a la base de datos '" + rutaBD + "'. Verifica que la ruta exista.");

    for (const string& t : tareas)
        bd << ++ultimoId << "|" << t << "\n";
    if (!bd)
        throw BaseDatosError("Error al escribir en la base de datos '" + rutaBD + "'.");
    bd.close();

    cout << "[OK] " << tareas.size() << " tareas guardadas. Contenido de la base de datos:" << endl;
    ifstream lectura(rutaBD);
    while (getline(lectura, linea)) {
        size_t sep = linea.find('|');
        if (sep != string::npos)
            cout << "  " << linea.substr(0, sep) << ". " << linea.substr(sep + 1) << endl;
    }
}

int main() {
    string archivo = leerTexto("Nombre del archivo de tareas: ");
    string bd = leerTexto("Nombre de la base de datos: ");

    try {
        vector<string> tareas = leerTareas(archivo);
        guardarEnBD(bd, tareas);
        cout << "Proceso completado con exito." << endl;
    } catch (const ArchivoError& e) {
        cout << "[ERROR] " << e.what() << endl;
        cout << "El programa termino de forma controlada." << endl;
    } catch (const BaseDatosError& e) {
        cout << "[ERROR] " << e.what() << endl;
        cout << "El programa termino de forma controlada." << endl;
    }
    return 0;
}