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
        // Listar las cláusulas ingresadas
        {"listar_clausulas", [&]() {
            listarClausulas(clausulas);
        }},

        {"aceptar_clausulas", [&]() {
            running = false;  
            
            try {
                // Obtener las palabras de acción del usuario
                std::vector<std::string> palabrasAcciones = consegirPalabrasAcciones();
                
                // Mostrar las cláusulas ingresadas
                std::cout << "\nClausulas ingresadas:\n";
                listarClausulas(clausulas);
                
                // Procesar las cláusulas a formato lógico
                std::vector<ProposicionesConConectores> clausulasProcesadas = 
                    procesarCondicionalesLogicos(clausulas, palabrasAcciones);
                
                // Mostrar las proposiciones procesadas
                std::cout << "\nProposiciones procesadas:\n";
                for (const auto& proposicion : clausulasProcesadas) {
                    proposicion.mostrar();
                }
                
                // Obtener y preparar el teorema a demostrar
                Relacion* teorema = conseguirTeorema(palabrasAcciones);
                teorema->toggleNegacion();  
                std::cout << "\nTeorema a demostrar (negado):\n";
                teorema->mostrar();
                
                // Ejecutar la máquina de inferencia
                maquinaDeInferencia(clausulasProcesadas, teorema);
                delete teorema;
                
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
        
        // Buscar si el input es un comando conocido
        auto it = acciones.find(input);
        if (it != acciones.end()) {
            // Ejecutar la acción correspondiente
            it->second();
        } else if (!input.empty()) {
            // Procesar como cláusula si no está vacío
            palabras.clear();
            std::istringstream iss(input);
            std::string word;
            
            // Dividir la entrada en palabras
            while (iss >> word) {
                palabras.push_back(word);
            }
            
            // Agregar la cláusula si no está vacía
            if (!palabras.empty()) {
                clausulas.push_back(palabras);
            }
        }
    }
    
    return 0;
}


