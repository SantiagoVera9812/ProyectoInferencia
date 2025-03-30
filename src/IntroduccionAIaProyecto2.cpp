#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <functional>
#include "manejoDeClausulas.h"
#include "ProposicionesConConectores.h"
#include "manejoDeClausulas.h"
#include "maquinaDeInferencia.h"
#include "procesarConector.h"



int main() {
    
    std::string input;
    std::vector<std::string> palabras;
    std::vector<std::vector<std::string>> clausulas; 
    bool running = true;

    // Definicion de funciones fuera de las clausulas que permitiran al usuario interactuar con el programa
    std::unordered_map<std::string, std::function<void()>> acciones = {

        {"listar_clausulas", [&]() {

            listarClausulas(clausulas);

        }},

        //Cuando se tengan todas las clausulas intentar encontrar una solucion
        {"aceptar_clausulas", [&]() {

            std::cout << "Buscando solucion..." << std::endl;
            running = false;

            std::cout << "Escribe las acciones que representan un accionar, escribe aceptar_acciones para terminar" << std::endl;

            bool escribiendoAcciones = true;

            std::vector<std::string> palabrasAcciones = consegirPalabrasAcciones();

            std::cout << "Acciones: " << std::endl;

            for (const auto& w : palabrasAcciones) {
                std::cout << w << std::endl;
            }

            listarClausulas(clausulas);

            std::vector<ProposicionesConConectores> clausulasProcesadas = procesarCondicionalesLogicos(clausulas, palabrasAcciones);

            for (const auto& proposicion : clausulasProcesadas) {
                std::cout << "en main vemos la proposicion:" << std::endl;
                proposicion.mostrar();
            }

            Relacion* teorema = conseguirTeorema(palabrasAcciones).toggleNegacion();

            teorema->mostrar();

            maquinaDeInferencia(clausulasProcesadas, teorema);




                }},

        //Salir del programa
        {"salir", [&]() {
            std::cout << "Saliendo del programa..." << std::endl;
            running = false;
            return 0;
        }}

    };

    while (running) {

        std::cout << "Escribe las cláusulas que realizará el programa, si haces algo que no sea escribir una clausula puedes: escribe listar_clausulas para listar las clausulas, aceptar_clausulas para seguir al siguiente paso y salir para salir del programa" << std::endl;
        std::getline(std::cin, input);

        auto it = acciones.find(input);
        if (it != acciones.end()) {
            it->second(); // Ejecuta la función correspondiente
        } else {
            // Si la entrada no está en el mapa, se trata como cláusula

            //Procesamiento de una clausula con un formato que entienda el programa

            palabras.clear();
            std::istringstream iss(input);
            std::string word;

            while (iss >> word) {
                palabras.push_back(word);
            }

            clausulas.push_back(palabras);

            std::cout << "Palabras separadas:" << std::endl;
            for (const auto& w : palabras) {
                std::cout << w << std::endl;
            }
        }
    };


    return 0;

    
}


