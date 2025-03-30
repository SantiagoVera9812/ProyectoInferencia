#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <sstream>
#include "Relacion.h"
#include "ProposicionesConConectores.h"


void listarClausulas(std::vector<std::vector<std::string>> clausulas){

    std::cout << "\nClausulas ingresadas para encontrar una solucion:\n";
            for (const auto& entry : clausulas) {
                for (const auto& w : entry) {
                    std::cout << w << " ";
                    }
                    std::cout << std::endl;
                    }

};

Relacion procesarClausulasLogicas(std::vector<std::string> clausulaRelacion, std::vector<std::string> palabrasAcciones) {
    std::unordered_map<std::string, std::function<Relacion(size_t, const std::vector<std::string>&, bool)>> accionesConectores;

    // Función para detectar variables (empiezan con ?)
    auto esVariable = [](const std::string& s) { return !s.empty() && s[0] == '?'; };

    // Acción para "es"
    accionesConectores["es"] = [&](size_t i, const std::vector<std::string>& clausula, bool esNegacion) -> Relacion {
        if (i > 0 && i + 1 < clausula.size()) {
            Term sujeto(clausula[i-1], esVariable(clausula[i-1]));
            Term valor(clausula[i+1], esVariable(clausula[i+1]));
            return Relacion("definicion", {sujeto, valor}, esNegacion);
        }
        throw std::runtime_error("Invalid 'es' relationship: missing previous or next word.");
    };

    // Acción para "no"
    accionesConectores["no"] = [](size_t i, const std::vector<std::string>& clausula, bool) -> Relacion {
        throw std::runtime_error("Negación debe aplicarse a una relación");
    };

    // Procesar acciones dinámicas
    for (const std::string& accion : palabrasAcciones) {
        accionesConectores[accion] = [&, accion](size_t i, const std::vector<std::string>& clausula, bool esNegacion) -> Relacion {
            if (i > 0 && i + 1 < clausula.size()) {
                Term sujeto(clausula[i-1], esVariable(clausula[i-1]));
                Term objeto(clausula[i+1], esVariable(clausula[i+1]));
                return Relacion(accion, {sujeto, objeto}, esNegacion);
            }
            throw std::runtime_error("Invalid action relationship: missing previous or next word.");
        };
    }

    bool esNegacion = false;
    size_t inicioRelacion = 0;

    for (size_t i = 0; i < clausulaRelacion.size(); ++i) {
        if (clausulaRelacion[i] == "no") {
            esNegacion = true;
            inicioRelacion = i+1;
            continue;
        }

        auto it = accionesConectores.find(clausulaRelacion[i]);
        if (it != accionesConectores.end()) {
            // Verificar si hay negación antes de la relación
            if (i > inicioRelacion) {
                std::vector<std::string> relacion(clausulaRelacion.begin() + inicioRelacion, clausulaRelacion.begin() + i + 1);
                return it->second(i - inicioRelacion, relacion, esNegacion);
            }
            return it->second(i, clausulaRelacion, esNegacion);
        }
    }

    throw std::runtime_error("No valid relationship found in the clause.");
}

std::vector<std::string> consegirPalabrasAcciones(){

    bool escribiendoAcciones = true;

            std::vector<std::string> palabrasAcciones;

            while(escribiendoAcciones){

                std::string input;
                std::getline(std::cin, input);

                if(input == "aceptar_acciones"){
                    escribiendoAcciones = false;
                }

                palabrasAcciones.push_back(input);

            }

            return palabrasAcciones;
}


Relacion conseguirTeorema(std::vector<std::string> palabrasAcciones){


    std::cout << "Escribe una relacion valida que funcione como teorema: " << std::endl;
    std::vector<std::string> palabras;

    std::string input;
    std::getline(std::cin, input);

    std::istringstream iss(input);
            std::string word;

            while (iss >> word) {
                palabras.push_back(word);
            }

    return procesarClausulasLogicas(palabras, palabrasAcciones);

}

