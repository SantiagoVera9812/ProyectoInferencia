#include <stack>
#include <vector>
#include <tuple>
#include <iostream>
#include "maquinaDeInferencia.h"
#include "ProposicionesConConectores.h"
#include "Unificacion.h"

bool sonComplementarias(const Relacion& a, const Relacion& b, Unificacion& unificacion) {
    if (a.getNombreFuncion() != b.getNombreFuncion()) {
        return false;
    }
    
    // Verificar si una es negación de la otra
    if (a.getEsNegacion() == b.getEsNegacion()) {
        return false;
    }
    
    // Intentar unificar
    Unificacion tempUnif = unificacion;
    if (!tempUnif.unificar(a, b)) {
        return false;
    }
    
    // Si la unificación fue exitosa, aplicar las sustituciones
    unificacion.combinar(tempUnif);
    return true;
}
void maquinaDeInferencia(std::vector<ProposicionesConConectores> clausulasProcesadas, Relacion* teorema) {
    if (teorema == nullptr) {
        std::cerr << "Error: Teorema inválido (puntero nulo)\n";
        return;
    }

    ProposicionesConConectores proposicionActual;
    proposicionActual.agregarRelacion(teorema);

    std::stack<std::tuple<std::vector<ProposicionesConConectores>, 
                         ProposicionesConConectores, 
                         Unificacion>> estados;
    
    estados.push(std::make_tuple(clausulasProcesadas, proposicionActual, Unificacion()));

    while (!estados.empty()) {
        auto estado = estados.top();
        estados.pop();
        
        auto estadoActual = std::get<0>(estado);
        auto proposicionActual = std::get<1>(estado);
        auto unificacion = std::get<2>(estado);

        // Aplicar sustituciones
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

        if (estadoActual.empty()) {
            std::cout << "✅ El teorema ha sido demostrado!\n";
            return;
        }

        bool cambioRealizado = false;
        for (size_t i = 0; i < estadoActual.size() && !cambioRealizado; ++i) {
            auto& clausula = estadoActual[i];
            auto& relacionesClausula = clausula.getRelaciones();

            for (size_t j = 0; j < relacionesClausula.size() && !cambioRealizado; ++j) {
                auto& relClausula = relacionesClausula[j];

                for (size_t k = 0; k < proposicionActual.getRelaciones().size() && !cambioRealizado; ++k) {
                    auto& relActual = proposicionActual.getRelaciones()[k];
                    
                    Unificacion nuevaUnificacion = unificacion;
                    if (sonComplementarias(relClausula, relActual, nuevaUnificacion)) {
                        std::cout << "\n⚡ Relaciones complementarias encontradas:\n";
                        relClausula.mostrar();
                        relActual.mostrar();

                        auto nuevoEstado = estadoActual;
                        auto nuevaProposicion = proposicionActual;

                        nuevoEstado[i].eliminarRelacion(j);
                        nuevaProposicion.eliminarRelacion(k);

                        if (nuevoEstado[i].getRelaciones().empty()) {
                            nuevoEstado.erase(nuevoEstado.begin() + i);
                        }

                        if (nuevaProposicion.getRelaciones().empty()) {
                            if (nuevoEstado.empty()) {
                                std::cout << "✅ El teorema ha sido demostrado!\n";
                                return;
                            }
                            nuevaProposicion = nuevoEstado[0];
                            nuevoEstado.erase(nuevoEstado.begin());
                        }

                        estados.push(std::make_tuple(nuevoEstado, nuevaProposicion, nuevaUnificacion));
                        cambioRealizado = true;
                    }
                }
            }
        }
    }

    std::cout << "❌ No se pudo demostrar el teorema.\n";
}



