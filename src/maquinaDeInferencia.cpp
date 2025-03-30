
#include <stack>
#include <vector>
#include "maquinaDeInferencia.h"
#include "ProposicionesConConectores.h"
#include "Unificacion.h"

void maquinaDeInferencia(std::vector<ProposicionesConConectores> clausulasProcesadas, Relacion* teorema) {
    // Convertir el teorema a forma negada
    ProposicionesConConectores objetivo;
    objetivo.agregarRelacion(teorema->toggleNegacion());

    // Pila para backtracking: {clausulas, objetivo, sustitucion}
    std::stack<std::tuple<std::vector<ProposicionesConConectores>, 
                         ProposicionesConConectores,
                         std::unordered_map<std::string, Term>>> pila;
    
    pila.push({clausulasProcesadas, objetivo, {}});

    while (!pila.empty()) {
        auto [clausulasRestantes, objetivoActual, sustitucion] = pila.top();
        pila.pop();

        // Mostrar estado actual
        std::cout << "\nObjetivo actual: ";
        objetivoActual.mostrar();
        std::cout << "\nClausulas restantes: " << clausulasRestantes.size() << "\n";

        // Si no hay objetivo, hemos encontrado contradicción
        if (objetivoActual.getRelaciones().empty()) {
            std::cout << "✅ Contradicción encontrada - Teorema demostrado!\n";
            if (!sustitucion.empty()) {
                std::cout << "Sustituciones aplicadas:\n";
                for (const auto& [var, term] : sustitucion) {
                    std::cout << "?" << var << " = ";
                    term.mostrar();
                    std::cout << "\n";
                }
            }
            return;
        }

        // Tomar el primer literal del objetivo
        Relacion litObjetivo = objetivoActual.getRelaciones()[0];
        ProposicionesConConectores nuevoObjetivo = objetivoActual;
        nuevoObjetivo.eliminarRelacion(0);

        // Buscar en las cláusulas
        for (size_t i = 0; i < clausulasRestantes.size(); ++i) {
            auto clausula = clausulasRestantes[i];
            
            for (size_t j = 0; j < clausula.getRelaciones().size(); ++j) {
                Relacion litClausula = clausula.getRelaciones()[j];
                
                // Intentar unificación
                std::unordered_map<std::string, Term> nuevaSust = sustitucion;
                if (Unificador::unificar(litObjetivo, litClausula, nuevaSust)) {
                    std::cout << "⚡ Unificación exitosa entre:\n  ";
                    litObjetivo.mostrar();
                    std::cout << "\n  y\n  ";
                    litClausula.mostrar();
                    std::cout << "\n";

                    // Aplicar sustitución a la cláusula
                    ProposicionesConConectores clausulaUnificada;
                    for (const auto& rel : clausula.getRelaciones()) {
                        if (!(rel == litClausula)) {
                            clausulaUnificada.agregarRelacion(
                                new Relacion(rel.aplicarSustitucion(nuevaSust)));
                        }
                    }

                    // Aplicar sustitución al objetivo restante
                    ProposicionesConConectores objetivoUnificado;
                    for (const auto& rel : nuevoObjetivo.getRelaciones()) {
                        objetivoUnificado.agregarRelacion(
                            new Relacion(rel.aplicarSustitucion(nuevaSust)));
                    }

                    // Combinar con la cláusula unificada
                    for (const auto& rel : clausulaUnificada.getRelaciones()) {
                        objetivoUnificado.agregarRelacion(new Relacion(rel));
                    }

                    // Crear nuevo conjunto de cláusulas (eliminando la usada)
                    auto nuevasClausulas = clausulasRestantes;
                    nuevasClausulas.erase(nuevasClausulas.begin() + i);

                    // Agregar a la pila
                    pila.push({nuevasClausulas, objetivoUnificado, nuevaSust});
                }
            }
        }
    }

    std::cout << "❌ No se pudo demostrar el teorema\n";
}




