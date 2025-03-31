#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <functional>
#include "manejoDeClausulas.h"
#include "ProposicionesConConectores.h"
#include "maquinaDeInferencia.h"
#include "procesarConector.h"

int main() {  
    std::string input;
    std::vector<std::string> palabras;
    std::vector<std::vector<std::string>> clausulas; 
    bool running = true;

    std::unordered_map<std::string, std::function<void()>> acciones = {
        {"listar_clausulas", [&]() {
            listarClausulas(clausulas);
        }},
        {"aceptar_clausulas", [&]() {
            running = false;
            try {
                std::vector<std::string> palabrasAcciones = consegirPalabrasAcciones();
                
                std::cout << "\nClausulas ingresadas:\n";
                listarClausulas(clausulas);
                
                std::vector<ProposicionesConConectores> clausulasProcesadas = 
                    procesarCondicionalesLogicos(clausulas, palabrasAcciones);
                
                std::cout << "\nProposiciones procesadas:\n";
                for (const auto& proposicion : clausulasProcesadas) {
                    proposicion.mostrar();
                }
                
                Relacion* teorema = conseguirTeorema(palabrasAcciones);
                teorema->toggleNegacion();
                std::cout << "\nTeorema a demostrar (negado):\n";
                teorema->mostrar();
                
                maquinaDeInferencia(clausulasProcesadas, teorema);
                
                delete teorema;  // Liberar memoria
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << "\n";
            }
        }},
        {"salir", [&]() {
            running = false;
        }}
    };

    while (running) {
        std::cout << "\nIngrese clausula o comando (listar_clausulas/aceptar_clausulas/salir): ";
        std::getline(std::cin, input);
        
        auto it = acciones.find(input);
        if (it != acciones.end()) {
            it->second();
        } else if (!input.empty()) {
            palabras.clear();
            std::istringstream iss(input);
            std::string word;
            while (iss >> word) {
                palabras.push_back(word);
            }
            if (!palabras.empty()) {
                clausulas.push_back(palabras);
            }
        }
    }
    
    return 0;
}


