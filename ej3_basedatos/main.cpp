// Ejercicio 3: Base de datos en archivo (tabla productos) - insertar y consultar datos del usuario
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

struct Producto {
    int id;
    string nombre;
    double precio;
    int stock;
};

// Clase que simula una tabla: cada registro es una linea "id|nombre|precio|stock"
class BaseDatos {
private:
    string ruta;

public:
    BaseDatos(const string& ruta) : ruta(ruta) {}

    bool conectar() {
        ofstream archivo(ruta, ios::app); // crea el archivo si no existe
        return archivo.good();
    }

    vector<Producto> seleccionar(double precioMin = 0) {
        vector<Producto> resultado;
        ifstream archivo(ruta);
        string linea;
        while (getline(archivo, linea)) {
            if (linea.empty()) continue;
            stringstream ss(linea);
            string campo;
            Producto p;
            getline(ss, campo, '|'); p.id = stoi(campo);
            getline(ss, p.nombre, '|');
            getline(ss, campo, '|'); p.precio = stod(campo);
            getline(ss, campo, '|'); p.stock = stoi(campo);
            if (p.precio >= precioMin) resultado.push_back(p);
        }
        return resultado;
    }

    int insertar(const string& nombre, double precio, int stock) {
        vector<Producto> todos = seleccionar();
        int nuevoId = todos.empty() ? 1 : todos.back().id + 1; // id autoincrementable
        ofstream archivo(ruta, ios::app);
        archivo << nuevoId << "|" << nombre << "|" << precio << "|" << stock << "\n";
        return nuevoId;
    }
};

string leerTexto(const string& msg) {
    cout << msg;
    string s;
    getline(cin, s);
    return s;
}

int leerEntero(const string& msg) {
    int n;
    cout << msg;
    while (!(cin >> n)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Valor invalido. " << msg;
    }
    cin.ignore(10000, '\n');
    return n;
}

double leerDecimal(const string& msg) {
    double d;
    cout << msg;
    while (!(cin >> d)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Valor invalido. " << msg;
    }
    cin.ignore(10000, '\n');
    return d;
}

void mostrar(const vector<Producto>& productos) {
    cout << left << setw(5) << "ID" << setw(22) << "Nombre"
         << right << setw(10) << "Precio" << setw(8) << "Stock" << endl;
    cout << string(45, '-') << endl;
    if (productos.empty()) cout << "(sin resultados)" << endl;
    for (const Producto& p : productos)
        cout << left << setw(5) << p.id << setw(22) << p.nombre
             << right << fixed << setprecision(2) << setw(10) << p.precio
             << setw(8) << p.stock << endl;
}

int main() {
    BaseDatos db("productos.db");
    if (!db.conectar()) {
        cout << "No se pudo conectar a la base de datos." << endl;
        return 1;
    }
    cout << "Conectado a productos.db" << endl;

    int opcion;
    do {
        cout << endl << "=== Productos ===" << endl;
        cout << "1. Insertar producto" << endl;
        cout << "2. Ver todos los productos" << endl;
        cout << "3. Buscar productos por precio minimo" << endl;
        cout << "4. Salir" << endl;
        opcion = leerEntero("Opcion: ");
        if (opcion == 1) {
            string nombre;
            do {
                nombre = leerTexto("Nombre del producto: ");
            } while (nombre.empty() || nombre.find('|') != string::npos);
            double precio = leerDecimal("Precio: ");
            int stock = leerEntero("Stock: ");
            int id = db.insertar(nombre, precio, stock);
            cout << "Producto insertado con ID " << id << "." << endl;
        } else if (opcion == 2) {
            mostrar(db.seleccionar());
        } else if (opcion == 3) {
            mostrar(db.seleccionar(leerDecimal("Precio minimo: ")));
        } else if (opcion == 4) {
            cout << "Adios." << endl;
        } else {
            cout << "Opcion no valida." << endl;
        }
    } while (opcion != 4);
    return 0;
}