// Ejercicio 1: Validacion con excepciones (edad y correo) con datos del usuario
#include <iostream>
#include <string>
#include <regex>
#include <stdexcept>
using namespace std;

// Excepciones propias para cada tipo de error
class EdadInvalidaError : public runtime_error {
public:
    explicit EdadInvalidaError(const string& msg) : runtime_error(msg) {}
};

class CorreoInvalidoError : public runtime_error {
public:
    explicit CorreoInvalidoError(const string& msg) : runtime_error(msg) {}
};

string leerTexto(const string& msg) {
    cout << msg;
    string s;
    getline(cin, s);
    return s;
}

int validarEdad(const string& texto) {
    size_t pos = 0;
    int edad;
    try {
        edad = stoi(texto, &pos);
    } catch (const invalid_argument&) {
        throw EdadInvalidaError("'" + texto + "' no es un numero entero.");
    } catch (const out_of_range&) {
        throw EdadInvalidaError("'" + texto + "' es un numero demasiado grande.");
    }
    if (pos != texto.size())
        throw EdadInvalidaError("'" + texto + "' contiene caracteres que no son numeros.");
    if (edad < 0 || edad > 120)
        throw EdadInvalidaError("La edad " + to_string(edad) + " esta fuera del rango permitido (0-120).");
    return edad;
}

string validarCorreo(const string& texto) {
    static const regex patron(R"(^[\w.+-]+@[\w-]+(\.[\w-]+)*\.[A-Za-z]{2,}$)");
    if (!regex_match(texto, patron))
        throw CorreoInvalidoError("'" + texto + "' no tiene el formato usuario@dominio.com.");
    return texto;
}

int main() {
    cout << "=== Registro de usuario ===" << endl;
    string nombre = leerTexto("Nombre: ");
    int edad;
    string correo;

    // Se repite hasta que validarEdad no lance excepcion
    while (true) {
        try {
            edad = validarEdad(leerTexto("Edad: "));
            break;
        } catch (const EdadInvalidaError& e) {
            cout << "  [ERROR] " << e.what() << " Intenta de nuevo." << endl;
        }
    }

    while (true) {
        try {
            correo = validarCorreo(leerTexto("Correo electronico: "));
            break;
        } catch (const CorreoInvalidoError& e) {
            cout << "  [ERROR] " << e.what() << " Intenta de nuevo." << endl;
        }
    }

    cout << endl << "=== Datos validos ===" << endl;
    cout << "Nombre: " << nombre << " | Edad: " << edad << " | Correo: " << correo << endl;
    return 0;
}