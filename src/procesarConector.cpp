#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "procesarConector.h"
#include "manejoDeClausulas.h"
#include "ProposicionesConConectores.h"

// Función auxiliar para separar una cláusula por un conector específico
std::vector<std::vector<std::string>> separarPorConector(const std::vector<std::string>& clausula, const std::string& conector) {
    std::vector<std::vector<std::string>> resultado;
    std::vector<std::string> buffer;
    
    for (const auto& palabra : clausula) {
        if (palabra == conector) {
            if (!buffer.empty()) {
                resultado.push_back(buffer);
                buffer.clear();
            }
        } else {
            buffer.push_back(palabra);
        }
    }
    
    if (!buffer.empty()) {
        resultado.push_back(buffer);
    }
    
    return resultado;
}

// Función para negar un literal (agrega o quita "no")
std::vector<std::string> negarLiteral(std::vector<std::string> literal) {
    if (!literal.empty() && literal[0] == "no") {
        literal.erase(literal.begin());
    } else {
        literal.insert(literal.begin(), "no");
    }
    return literal;
}

// Función para negar una cláusula completa
std::vector<std::string> negarClausula(std::vector<std::string> clausulaANegar, std::vector<std::string>) {
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

// Procesa cláusulas con conectores "y"
std::vector<std::vector<std::string>> procesarConectorLogicoY(std::vector<std::string> clausula) {
    return separarPorConector(clausula, "y");
}

// Procesa cláusulas básicas (sin condicionales)
ProposicionesConConectores procesarConectorLogico0(std::vector<std::string> clausula, std::vector<std::string> palabrasAcciones) {
    std::vector<std::string> bufferProposicion;
    std::vector<Relacion> relacionesBuffer;
    std::string conectorActual = "";

    for (size_t i = 0; i < clausula.size(); ++i) {
        if (clausula[i] == "o") {
            if (!bufferProposicion.empty()) {
                try {
                    Relacion relacion = procesarClausulasLogicas(bufferProposicion, palabrasAcciones);
                    relacionesBuffer.push_back(relacion);
                } catch (const std::exception& e) {
                    std::cerr << "Error procesando parte de cláusula: " << e.what() << "\n";
                }
                bufferProposicion.clear();
            }
            conectorActual = clausula[i];
        } else {
            bufferProposicion.push_back(clausula[i]);
        }
    }

    if (!bufferProposicion.empty()) {
        try {
            Relacion relacion = procesarClausulasLogicas(bufferProposicion, palabrasAcciones);
            relacionesBuffer.push_back(relacion);
        } catch (const std::exception& e) {
            std::cerr << "Error procesando parte de cláusula: " << e.what() << "\n";
        }
    }

    if (!relacionesBuffer.empty()) {
        return ProposicionesConConectores(conectorActual, relacionesBuffer);
    }
    throw std::runtime_error("Cláusula vacía o no procesable");
}

// Función principal que procesa todos los condicionales lógicos
std::vector<ProposicionesConConectores> procesarCondicionalesLogicos(
    std::vector<std::vector<std::string>> clausulas, 
    std::vector<std::string> palabrasAcciones) {
    
    std::vector<ProposicionesConConectores> proposiciones;
    
    for (const auto& clausula : clausulas) {
        // Buscar "si" y "entonces"
        auto itSi = std::find(clausula.begin(), clausula.end(), "si");
        auto itEntonces = std::find(clausula.begin(), clausula.end(), "entonces");
        
        if (itSi != clausula.end() && itEntonces != clausula.end()) {
            // Separar antecedente y consecuente
            std::vector<std::string> antecedente(itSi + 1, itEntonces);
            std::vector<std::string> consecuente(itEntonces + 1, clausula.end());
            
            // Procesar antecedente (parte después de "si")
            auto partesAntecedente = separarPorConector(antecedente, "y");
            
            // Procesar consecuente (parte después de "entonces")
            auto partesConsecuente = separarPorConector(consecuente, "o");
            
            // Convertir a forma normal conjuntiva
            for (const auto& parteConsecuente : partesConsecuente) {
                std::vector<std::string> nuevaClausula;
                
                // Negar cada parte del antecedente
                for (const auto& parteAntecedente : partesAntecedente) {
                    auto negado = negarLiteral(parteAntecedente);
                    nuevaClausula.insert(nuevaClausula.end(), negado.begin(), negado.end());
                    nuevaClausula.push_back("o");
                }
                
                // Añadir parte del consecuente
                nuevaClausula.insert(nuevaClausula.end(), parteConsecuente.begin(), parteConsecuente.end());
                
                // Procesar la nueva cláusula
                try {
                    ProposicionesConConectores prop = procesarConectorLogico0(nuevaClausula, palabrasAcciones);
                    proposiciones.push_back(prop);
                } catch (const std::exception& e) {
                    std::cerr << "Error procesando cláusula derivada: " << e.what() << "\n";
                }
            }
        } else {
            // Procesar cláusulas sin condicionales
            try {
                ProposicionesConConectores prop = procesarConectorLogico0(clausula, palabrasAcciones);
                proposiciones.push_back(prop);
            } catch (const std::exception& e) {
                std::cerr << "Error procesando cláusula simple: " << e.what() << "\n";
            }
        }
    }
    
    return proposiciones;
}





