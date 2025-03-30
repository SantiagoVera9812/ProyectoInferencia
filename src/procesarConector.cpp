#include <vector>
#include <string>
#include "manejoDeClausulas.h"
#include "ProposicionesConConectores.h"
#include "procesarConector.h"

std::vector<std::string> negarLiteral(std::vector<std::string> literal);
std::vector<std::string> negarClausula(std::vector<std::string> clausulaANegar, std::vector<std::string> palabrasAcciones);
std::vector<ProposicionesConConectores> procesarConectoresLogicosNormalizados(std::vector<std::vector<std::string>> clausulas, std::vector<std::string> palabrasAcciones);
ProposicionesConConectores procesarConectorLogico0(std::vector<std::string> clausula, std::vector<std::string> palabrasAcciones);
std::vector<std::vector<std::string>> procesarConectorLogicoY(std::vector<std::string> clausula);

std::vector<ProposicionesConConectores> procesarCondicionalesLogicos(std::vector<std::vector<std::string>> clausulas, std::vector<std::string> palabrasAcciones) {
    std::vector<ProposicionesConConectores> proposiciones;
    std::vector<std::string> bufferSi;
    std::vector<std::string> bufferEntonces;
    bool procesandoSi = false;
    bool procesandoEntonces = false;

    for (const auto& clausula : clausulas) {
        bufferSi.clear();
        bufferEntonces.clear();
        procesandoSi = false;
        procesandoEntonces = false;

        for (size_t i = 0; i < clausula.size(); ++i) {
            if (clausula[i] == "si") {
                if (procesandoSi || procesandoEntonces) {
                    throw std::runtime_error("Error: Se encontró 'si' sin un 'entonces' correspondiente.");
                }
                procesandoSi = true;
                bufferSi.clear();
            } else if (clausula[i] == "entonces") {
                if (!procesandoSi) {
                    throw std::runtime_error("Error: Se encontró 'entonces' sin un 'si' previo.");
                }
                procesandoEntonces = true;
                procesandoSi = false;
                bufferEntonces.clear();
            } else {
                if (procesandoSi) {
                    bufferSi.push_back(clausula[i]);
                } else if (procesandoEntonces) {
                    bufferEntonces.push_back(clausula[i]);
                } else {
                    continue;
                }
            }
        }

        if (!bufferSi.empty() && !bufferEntonces.empty()) {
            try {

                std::cout << "En buffers no vacios " << std::endl;
                std::vector<std::string> relacionSiNegada = negarClausula(bufferSi, palabrasAcciones);
                relacionSiNegada.push_back("o");

                std::cout << "Revisamos si entonces al negar con negarClausula" << std::endl;

                for(const auto& palabra: relacionSiNegada){
                    std::cout << palabra << std::endl;
                }

                // Concatenar los elementos de relacionSiNegada con el buffer de entonces
                relacionSiNegada.insert(relacionSiNegada.end(), bufferEntonces.begin(), bufferEntonces.end());

                std::cout << "Revisamos si entonces al realizar un insert" << std::endl;
                for(const auto& palabra: relacionSiNegada){
                    std::cout << palabra << std::endl;
                }

                std::vector<std::vector<std::string>>vectorClausulaY = procesarConectorLogicoY(relacionSiNegada);

                for(const auto& nuevaClausula: vectorClausulaY){
                   
                    ProposicionesConConectores proposicionSiEntonces = procesarConectorLogico0(nuevaClausula, palabrasAcciones);
                    proposicionSiEntonces.mostrar();

                    proposiciones.emplace_back(proposicionSiEntonces);
                }

                
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }

        

        if (bufferSi.empty() && bufferEntonces.empty()) {
            try {

                std::vector<std::vector<std::string>>vectorClausulaY = procesarConectorLogicoY(clausula);

                for(const auto& nuevaClausula: vectorClausulaY){


                   
                    ProposicionesConConectores proposicionSiEntonces = procesarConectorLogico0(nuevaClausula, palabrasAcciones);
                    proposicionSiEntonces.mostrar();

                    proposiciones.emplace_back(proposicionSiEntonces);
                }
                
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        } 

        
    }   

    return proposiciones;
}


std::vector<std::string> negarLiteral(std::vector<std::string> literal) {

    if (!literal.empty() && literal[0] == "no") {
        literal.erase(literal.begin()); // Eliminar "no" si ya está presente
    } else {
        literal.insert(literal.begin(), "no"); // Agregar "no" al inicio si no está presente
    }
    return literal;
}


std::vector<std::string> negarClausula(std::vector<std::string> clausulaANegar, std::vector<std::string> palabrasAcciones) {
    std::vector<std::string> resultado;
    std::vector<std::string> buffer;
    std::string operadorActual = "";
    
    for (size_t i = 0; i < clausulaANegar.size(); ++i) {
        if (clausulaANegar[i] == "o" || clausulaANegar[i] == "y") {
            if (!buffer.empty()) {
                std::vector<std::string> negado = negarLiteral(buffer);
                resultado.insert(resultado.end(), negado.begin(), negado.end());
                resultado.push_back(operadorActual == "o" ? "y" : "o");
                buffer.clear();
            }
            operadorActual = clausulaANegar[i];
        } else {
            buffer.push_back(clausulaANegar[i]);
        }
    }
    
    if (!buffer.empty()) {
        std::vector<std::string> negado = negarLiteral(buffer);
        resultado.insert(resultado.end(), negado.begin(), negado.end());
    }
    
    return resultado;

}



std::vector<ProposicionesConConectores> procesarConectoresLogicosNormalizados(std::vector<std::vector<std::string>> clausulas, std::vector<std::string> palabrasAcciones) {
    std::vector<ProposicionesConConectores> proposiciones;
   // Guardará el conector que está separando las proposiciones

    for (const auto& clausula : clausulas) {
        
        ProposicionesConConectores proposicion = procesarConectorLogico0(clausula, palabrasAcciones);

        proposiciones.emplace_back(proposicion);

    }

    return proposiciones;

}


ProposicionesConConectores procesarConectorLogico0(std::vector<std::string> clausula, std::vector<std::string> palabrasAcciones) {

    std::vector<std::string> bufferProposicion;
    std::vector<Relacion> relacionesBuffer;
    std::string conectorActual = ""; // Guardará el conector que está separando las proposiciones

    
        bufferProposicion.clear();
        relacionesBuffer.clear();
        conectorActual = "";

        for (size_t i = 0; i < clausula.size(); ++i) {
            if (clausula[i] == "o") {
                // Si encontramos "o", procesamos lo que se acumuló antes
                if (!bufferProposicion.empty()) {
                    try {
                        
                        Relacion relacion = procesarClausulasLogicas(bufferProposicion, palabrasAcciones);
                        relacionesBuffer.push_back(relacion);
                    } catch (const std::exception& e) {
                        std::cerr << "Error: " << e.what() << std::endl;
                    }
                    bufferProposicion.clear(); // Resetear el buffer
                }
                conectorActual = clausula[i]; // Asignamos "o" como conector
            } else {
                bufferProposicion.push_back(clausula[i]);
            }
        }

        // Procesar la última proposición de la cláusula si no termina en "o"
        if (!bufferProposicion.empty()) {
            try {
                Relacion relacion = procesarClausulasLogicas(bufferProposicion, palabrasAcciones);
                relacionesBuffer.push_back(relacion);
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }

        // craer Propocisiom
        if (!relacionesBuffer.empty()) {
            return ProposicionesConConectores(conectorActual, relacionesBuffer);
        }else {
            throw std::runtime_error("Error: No se encontraron relaciones en la cláusula.");
        }
    
    
}


std::vector<std::vector<std::string>> procesarConectorLogicoY(
    std::vector<std::string> clausula){

        std::vector<std::vector<std::string>> resultado;
        std::vector<std::string> buffer;

        for(const auto& palabra: clausula){
            if(palabra == "y"){

                if(!buffer.empty()){
                    resultado.push_back(buffer);
                    buffer.clear();
                }
            } else {
                buffer.push_back(palabra);
            }
        } 

        if(!buffer.empty()){
            resultado.push_back(buffer);
        }

        if(resultado.empty() && !clausula.empty()){
            resultado.push_back(clausula);
        }

        return resultado;
    }





