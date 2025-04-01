#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <sstream>
#include "manejoDeClausulas.h"
#include "Relacion.h"
#include "ProposicionesConConectores.h"

// Función para mostrar las cláusulas ingresadas
void listarClausulas(std::vector<std::vector<std::string>> clausulas) {
    std::cout << "\nClausulas ingresadas:\n";
    for (const auto& clausula : clausulas) {
        for (const auto& palabra : clausula) {
            std::cout << palabra << " ";
        }
        std::cout << "\n";
    }
}

// Función para obtener el teorema a demostrar desde la entrada del usuario
Relacion* conseguirTeorema(std::vector<std::string> palabrasAcciones) {
    std::cout << "\nIngrese el teorema a demostrar: ";
    std::string input;
    std::getline(std::cin, input);

    // Verificar que se ingresó algo
    if (input.empty()) {
        throw std::runtime_error("No se ingresó ningún teorema");
    }

    // Dividir la entrada en palabras individuales
    std::vector<std::string> palabras;
    std::istringstream iss(input);
    std::string word;
    while (iss >> word) {
        palabras.push_back(word);
    }

    try {
        // Procesar las palabras para crear una relación lógica
        return new Relacion(procesarClausulasLogicas(palabras, palabrasAcciones));
    } catch (const std::exception& e) {
        std::cerr << "Error procesando el teorema: " << e.what() << "\n";
        throw;
    }
}

// Función para procesar cláusulas lógicas
Relacion procesarClausulasLogicas(std::vector<std::string> clausulaRelacion, std::vector<std::string> palabrasAcciones) {
    std::unordered_map<std::string, std::function<Relacion(size_t, const std::vector<std::string>&, bool)>> accionesConectores;
    bool esNegacion = false;

    // Verificar si la cláusula comienza con "no" (negación)
    if (!clausulaRelacion.empty() && clausulaRelacion[0] == "no") {
        esNegacion = true;
        clausulaRelacion.erase(clausulaRelacion.begin()); // Eliminar el "no" para procesar el resto
    }

    if (clausulaRelacion.size() == 1) {
        size_t parenPos = clausulaRelacion[0].find('(');
        // Verificar si es un predicado con argumentos
        if (parenPos != std::string::npos && clausulaRelacion[0].back() == ')') {
            std::string nombrePred = clausulaRelacion[0].substr(0, parenPos);
            std::string argsStr = clausulaRelacion[0].substr(parenPos + 1, clausulaRelacion[0].size() - parenPos - 2);
            
            // Procesar los argumentos separados por comas
            std::vector<std::string> argumentos;
            std::istringstream iss(argsStr);
            std::string arg;
            while (std::getline(iss, arg, ',')) {
                // Limpiar espacios en blanco
                arg.erase(0, arg.find_first_not_of(" \t"));
                arg.erase(arg.find_last_not_of(" \t") + 1);
                argumentos.push_back(arg);
            }
            
            // Crear y devolver una relación de tipo predicado
            return Relacion("predicado", nombrePred, argumentos, esNegacion);
        }
    }

    // Se configuran los procesadores para diferentes tipos de relaciones

    // Procesador para el conector "es" (ej: "x es y")
    accionesConectores["es"] = [](size_t i, const std::vector<std::string>& clausula, bool esNegacion) -> Relacion {
        if (i > 0 && i + 1 < clausula.size()) {
            return Relacion("definicion", clausula[i + 1], {clausula[i - 1]}, esNegacion);
        }
        throw std::runtime_error("Formato inválido para 'es'");
    };

    // Procesadores para cada acción definida por el usuario
    for (const std::string& accion : palabrasAcciones) {
        accionesConectores[accion] = [accion](size_t i, const std::vector<std::string>& clausula, bool esNegacion) -> Relacion {
            if (i > 0 && i + 1 < clausula.size()) {
                return Relacion("accion", accion, {clausula[i - 1], clausula[i + 1]}, esNegacion);
            }
            throw std::runtime_error("Formato inválido para acción");
        };
    }

    // Buscar y aplicar el procesador adecuado para la cláusula
    for (size_t i = 0; i < clausulaRelacion.size(); ++i) {
        auto it = accionesConectores.find(clausulaRelacion[i]);
        if (it != accionesConectores.end()) {
            return it->second(i, clausulaRelacion, esNegacion);
        }
    }

    // Si no se encontró ningún procesador válido
    throw std::runtime_error("No se encontró una relación válida en la cláusula");
}

// Función para obtener las palabras de acción del usuario
std::vector<std::string> consegirPalabrasAcciones() {
    std::vector<std::string> palabrasAcciones;
    std::string input;
    
    std::cout << "\nIngrese las acciones (una por línea, 'aceptar_acciones' para terminar):\n";
    while (true) {
        std::getline(std::cin, input);
        if (input == "aceptar_acciones") break;
        if (!input.empty()) {
            palabrasAcciones.push_back(input);
        }
    }
    
    return palabrasAcciones;
}

