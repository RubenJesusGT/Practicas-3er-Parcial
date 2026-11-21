
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

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

int main() {
    json inventario;
    inventario["tienda"] = leerTexto("Nombre de la tienda: ");
    inventario["productos"] = json::array();

    int n;
    do {
        n = leerEntero("Cuantos productos quieres registrar? ");
    } while (n < 1);

    for (int i = 0; i < n; i++) {
        cout << endl << "--- Producto " << (i + 1) << " ---" << endl;
        json p;
        p["nombre"] = leerTexto("Nombre: ");
        p["precio"] = leerDecimal("Precio: ");
        p["cantidad"] = leerEntero("Cantidad: ");
        inventario["productos"].push_back(p);
    }

    // dump(4) convierte el objeto a texto JSON con sangria de 4 espacios
    ofstream salida("inventario.json");
    salida << inventario.dump(4);
    salida.close();
    cout << endl << "Archivo inventario.json creado:" << endl << inventario.dump(4) << endl;

    // Se vuelve a leer el archivo para comprobar que se guardo bien
    ifstream entrada("inventario.json");
    json leido = json::parse(entrada);

    double total = 0;
    cout << endl << "=== Inventario de " << leido["tienda"].get<string>() << " ===" << endl;
    cout << fixed << setprecision(2);
    for (const auto& p : leido["productos"]) {
        double subtotal = p["precio"].get<double>() * p["cantidad"].get<int>();
        total += subtotal;
        cout << "  " << left << setw(16) << p["nombre"].get<string>()
             << " x" << setw(5) << p["cantidad"].get<int>()
             << " = $" << subtotal << endl;
    }
    cout << "Valor total del inventario: $" << total << endl;
    return 0;
}