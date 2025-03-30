#include "Unificacion.h"

bool Unificador::unificar(const Term& t1, const Term& t2, 
                         std::unordered_map<std::string, Term>& sustitucion) {
    if (!t1.esVariable && !t2.esVariable) {
        return t1.nombre == t2.nombre;
    }
    
    if (t1.esVariable) {
        if (sustitucion.count(t1.nombre)) {
            return unificar(sustitucion.at(t1.nombre), t2, sustitucion);
        }
        
        if (t2.esVariable && sustitucion.count(t2.nombre)) {
            return unificar(t1, sustitucion.at(t2.nombre), sustitucion);
        }
        
        if (ocurreCheck(t1.nombre, t2, sustitucion)) {
            return false;
        }
        
        sustitucion[t1.nombre] = t2;
        return true;
    }
    
    return unificar(t2, t1, sustitucion);
}

bool Unificador::unificar(const Relacion& r1, const Relacion& r2,
                         std::unordered_map<std::string, Term>& sustitucion) {
    if (r1.getPredicado() != r2.getPredicado() || 
        r1.getTerminos().size() != r2.getTerminos().size()) {
        return false;
    }
    
    for (size_t i = 0; i < r1.getTerminos().size(); ++i) {
        if (!unificar(r1.getTerminos()[i], r2.getTerminos()[i], sustitucion)) {
            return false;
        }
    }
    
    return true;
}

bool Unificador::ocurreCheck(const std::string& var, const Term& term,
                           const std::unordered_map<std::string, Term>& sustitucion) {
    if (term.esVariable) {
        if (term.nombre == var) return true;
        auto it = sustitucion.find(term.nombre);
        if (it != sustitucion.end()) {
            return ocurreCheck(var, it->second, sustitucion);
        }
    }
    return false;
}