// Relacion.h
#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

class Term {
public:
    std::string nombre;
    bool esVariable;
    
    Term(std::string n = "", bool esVar = false);
    bool operator==(const Term& other) const;
    void mostrar() const;
    std::string toString() const;

    
};

class Relacion {
private:
    std::string predicado;
    std::vector<Term> terminos;
    bool esNegacion;

public:
    Relacion(std::string pred, std::vector<Term> terms, bool neg = false);
    Relacion();
    // Métodos para unificación
    bool unificar(const Relacion& otra, std::unordered_map<std::string, Term>& sustitucion) const;
    Relacion aplicarSustitucion(const std::unordered_map<std::string, Term>& sustitucion) const;
    bool esComplementaria(const Relacion& otra) const;
    
    // Métodos existentes
    Relacion* toggleNegacion();
    void mostrar() const;
    
    // Getters
    std::string getPredicado() const { return predicado; }
    std::vector<Term> getTerminos() const { return terminos; }
    bool getEsNegacion() const { return esNegacion; }
};

            



