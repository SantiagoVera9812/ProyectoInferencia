#pragma once
#include "Relacion.h"
#include <unordered_map>

class Unificador {
public:
    static bool unificar(const Term& t1, const Term& t2, 
                        std::unordered_map<std::string, Term>& sustitucion);
    
    static bool unificar(const Relacion& r1, const Relacion& r2,
                        std::unordered_map<std::string, Term>& sustitucion);
    
    static bool ocurreCheck(const std::string& var, const Term& term,
                          const std::unordered_map<std::string, Term>& sustitucion);
};