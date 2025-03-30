//relacion.cpp
#include "Relacion.h"
#include <iostream>
#include <unordered_map>

// Implementación de Term
Term::Term(std::string n, bool esVar) : nombre(n), esVariable(esVar) {}

bool Term::operator==(const Term& other) const {
    return nombre == other.nombre && esVariable == other.esVariable;
}

void Term::mostrar() const {
    std::cout << (esVariable ? "?" : "") << nombre;
}

std::string Term::toString() const {
    return (esVariable ? "?" : "") + nombre;
}

// Implementación de Relacion
Relacion::Relacion(std::string pred, std::vector<Term> terms, bool neg) : 
    predicado(pred), terminos(terms), esNegacion(neg) {}

bool Relacion::unificar(const Relacion& otra, std::unordered_map<std::string, Term>& sustitucion) const {
    if (predicado != otra.predicado || terminos.size() != otra.terminos.size()) {
        return false;
    }
    
    for (size_t i = 0; i < terminos.size(); ++i) {
        const Term& t1 = terminos[i];
        const Term& t2 = otra.terminos[i];
        
        if (t1.esVariable && t2.esVariable) {
            if (t1.nombre != t2.nombre) {
                sustitucion[t1.nombre] = t2;
            }
        } else if (t1.esVariable) {
            // Verificar ocurrencia
            if (t2.esVariable && sustitucion.count(t2.nombre)) {
                if (!(sustitucion[t2.nombre] == t1)) return false;
            } else {
                sustitucion[t1.nombre] = t2;
            }
        } else if (t2.esVariable) {
            // Verificar ocurrencia
            if (sustitucion.count(t2.nombre)) {
                if (!(sustitucion[t2.nombre] == t1)) return false;
            } else {
                sustitucion[t2.nombre] = t1;
            }
        } else if (t1.nombre != t2.nombre) {
            return false;
        }
    }
    return true;
}

Relacion Relacion::aplicarSustitucion(const std::unordered_map<std::string, Term>& sustitucion) const {
    std::vector<Term> nuevosTerminos;
    for (const auto& term : terminos) {
        if (term.esVariable && sustitucion.count(term.nombre)) {
            nuevosTerminos.push_back(sustitucion.at(term.nombre));
        } else {
            nuevosTerminos.push_back(term);
        }
    }
    return Relacion(predicado, nuevosTerminos, esNegacion);
}

bool Relacion::esComplementaria(const Relacion& otra) const {
    if (predicado != otra.predicado || terminos.size() != otra.terminos.size()) {
        return false;
    }
    
    for (size_t i = 0; i < terminos.size(); ++i) {
        if (!(terminos[i] == otra.terminos[i])) {
            return false;
        }
    }
    
    return esNegacion != otra.esNegacion;
}

Relacion* Relacion::toggleNegacion() {
    esNegacion = !esNegacion;
    return this;
}

void Relacion::mostrar() const {
    if (esNegacion) std::cout << "¬";
    std::cout << predicado << "(";
    for (size_t i = 0; i < terminos.size(); ++i) {
        terminos[i].mostrar();
        if (i != terminos.size() - 1) std::cout << ", ";
    }
    std::cout << ")";
}

// Constructor por defecto
Relacion::Relacion() : predicado(""), terminos(), esNegacion(false) {}

// Destructor
Relacion::~Relacion() {}

