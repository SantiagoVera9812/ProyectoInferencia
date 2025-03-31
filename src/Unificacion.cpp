#include "Unificacion.h"
#include <algorithm>

Unificacion::Unificacion() {}

bool Unificacion::esVariable(const std::string& termino) const {
    return !termino.empty() && islower(termino[0]);
}

bool Unificacion::unificar(const Relacion& a, const Relacion& b) {
    if (a.getNombreFuncion() != b.getNombreFuncion() || 
        a.getArgumentos().size() != b.getArgumentos().size()) {
        return false;
    }

    for (size_t i = 0; i < a.getArgumentos().size(); ++i) {
        const std::string& term1 = a.getArgumentos()[i];
        const std::string& term2 = b.getArgumentos()[i];

        if (esVariable(term1)) {
            auto it = sustituciones.find(term1);
            if (it != sustituciones.end()) {
                if (it->second != term2) return false;
            } else {
                sustituciones[term1] = term2;
            }
        } else if (esVariable(term2)) {
            auto it = sustituciones.find(term2);
            if (it != sustituciones.end()) {
                if (it->second != term1) return false;
            } else {
                sustituciones[term2] = term1;
            }
        } else {
            if (term1 != term2) return false;
        }
    }
    return true;
}

Relacion Unificacion::aplicarSustitucion(const Relacion& rel) const {
    std::vector<std::string> nuevosArgumentos;
    for (const auto& arg : rel.getArgumentos()) {
        if (esVariable(arg)) {
            auto it = sustituciones.find(arg);
            nuevosArgumentos.push_back(it != sustituciones.end() ? it->second : arg);
        } else {
            nuevosArgumentos.push_back(arg);
        }
    }
    return Relacion(rel.getTipoRelacion(), rel.getNombreFuncion(), nuevosArgumentos, rel.getEsNegacion());
}

void Unificacion::combinar(const Unificacion& otra) {
    for (const auto& [var, val] : otra.sustituciones) {
        sustituciones[var] = val;
    }
}

void Unificacion::limpiar() {
    sustituciones.clear();
}

const std::unordered_map<std::string, std::string>& Unificacion::getSustituciones() const {
    return sustituciones;
}

void Unificacion::mostrarSustituciones() const {
    std::cout << "Sustituciones actuales:\n";
    for (const auto& [var, val] : sustituciones) {
        std::cout << var << " = " << val << "\n";
    }
}