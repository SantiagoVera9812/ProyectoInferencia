#include <stack>
#include <vector>
#include <tuple>
#include <iostream>
#include "maquinaDeInferencia.h"
#include "ProposicionesConConectores.h"
#include "Unificacion.h"

// Función que verifica si dos relaciones son complementarias (una es la negación de la otra)
// y pueden ser unificadas mediante sustitución de variables
bool sonComplementarias(const Relacion& a, const Relacion& b, Unificacion& unificacion) {
    // Primero verifica que tengan el mismo nombre de función
    if (a.getNombreFuncion() != b.getNombreFuncion()) {
        return false;
    }
    
    // Verifica que una sea negación de la otra (no ambas negadas o ambas no negadas)
    if (a.getEsNegacion() == b.getEsNegacion()) {
        return false;
    }
    
    // Intenta unificar las relaciones 
    Unificacion tempUnif = unificacion;
    if (!tempUnif.unificar(a, b)) {
        return false; // Si no se pueden unificar, no son complementarias
    }
    
    // Si la unificación fue exitosa, combina las sustituciones con las existentes
    unificacion.combinar(tempUnif);
    return true;
}

void maquinaDeInferencia(std::vector<ProposicionesConConectores> clausulasProcesadas, Relacion* teorema) {
    if (teorema == nullptr) {
        std::cerr << "Error: Teorema inválido (puntero nulo)\n";
        return;
    }

    // Prepara la proposición actual con el teorema a demostrar
    ProposicionesConConectores proposicionActual;
    proposicionActual.agregarRelacion(teorema);

    // Pila de estados para implementar búsqueda con backtracking
    // Cada estado contiene:
    // 1. Lista de cláusulas disponibles
    // 2. Proposición actual a resolver
    // 3. Sustituciones de unificación acumuladas
    std::stack<std::tuple<std::vector<ProposicionesConConectores>, 
                         ProposicionesConConectores, 
                         Unificacion>> estados;
    
    estados.push(std::make_tuple(clausulasProcesadas, proposicionActual, Unificacion()));

    while (!estados.empty()) {
        // Obtiene el estado actual de la pila
        auto estado = estados.top();
        estados.pop();
        
        // Descompone el estado en sus componentes
        auto estadoActual = std::get<0>(estado);
        auto proposicionActual = std::get<1>(estado);
        auto unificacion = std::get<2>(estado);

        // Aplica las sustituciones de unificación a todas las relaciones
        for (auto& clausula : estadoActual) {
            for (auto& relacion : clausula.getRelaciones()) {
                relacion = unificacion.aplicarSustitucion(relacion);
            }
        }
        for (auto& relacion : proposicionActual.getRelaciones()) {
            relacion = unificacion.aplicarSustitucion(relacion);
        }

        std::cout << "\nEstado actual:\n";
        for (const auto& prop : estadoActual) prop.mostrar();
        std::cout << "Proposición actual:\n";
        proposicionActual.mostrar();
        unificacion.mostrarSustituciones();

        // Condición de éxito en la que no hay más cláusulas por resolver
        if (estadoActual.empty()) {
            std::cout << "✅ El teorema ha sido demostrado!\n";
            return;
        }

        // Intenta encontrar relaciones complementarias para aplicar resolución
        bool cambioRealizado = false;
        for (size_t i = 0; i < estadoActual.size() && !cambioRealizado; ++i) {
            auto& clausula = estadoActual[i];
            auto& relacionesClausula = clausula.getRelaciones();

            for (size_t j = 0; j < relacionesClausula.size() && !cambioRealizado; ++j) {
                auto& relClausula = relacionesClausula[j];

                for (size_t k = 0; k < proposicionActual.getRelaciones().size() && !cambioRealizado; ++k) {
                    auto& relActual = proposicionActual.getRelaciones()[k];
                    
                    // Verifica si son complementarias y pueden unificarse
                    Unificacion nuevaUnificacion = unificacion;
                    if (sonComplementarias(relClausula, relActual, nuevaUnificacion)) {
                        std::cout << "\n⚡ Relaciones complementarias encontradas:\n";
                        relClausula.mostrar();
                        relActual.mostrar();

                        // Prepara el nuevo estado después de la resolución
                        auto nuevoEstado = estadoActual;
                        auto nuevaProposicion = proposicionActual;

                        // Elimina las relaciones resueltas
                        nuevoEstado[i].eliminarRelacion(j);
                        nuevaProposicion.eliminarRelacion(k);

                        // Maneja casos especiales cuando se vacían cláusulas
                        if (nuevoEstado[i].getRelaciones().empty()) {
                            nuevoEstado.erase(nuevoEstado.begin() + i);
                        }

                        if (nuevaProposicion.getRelaciones().empty()) {
                            if (nuevoEstado.empty()) {
                                std::cout << "✅ El teorema ha sido demostrado!\n";
                                return;
                            }
                            // Toma la primera cláusula restante como nueva proposición
                            nuevaProposicion = nuevoEstado[0];
                            nuevoEstado.erase(nuevoEstado.begin());
                        }

                        // Agrega el nuevo estado a la pila para continuar la búsqueda
                        estados.push(std::make_tuple(nuevoEstado, nuevaProposicion, nuevaUnificacion));
                        cambioRealizado = true;
                    }
                }
            }
        }
    }

    // Si se recorren todos los estados sin encontrar demostración
    std::cout << "❌ No se pudo demostrar el teorema.\n";
}



