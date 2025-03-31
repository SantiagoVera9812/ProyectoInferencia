#pragma once
#include "Relacion.h"
#include <unordered_map>

class Unificacion {
    private:
        std::unordered_map<std::string, std::string> sustituciones;
        
        bool esVariable(const std::string& termino) const;
    
    public:
        Unificacion();
        
        bool unificar(const Relacion& a, const Relacion& b);
        Relacion aplicarSustitucion(const Relacion& rel) const;
        void combinar(const Unificacion& otra);
        void limpiar();
        
        const std::unordered_map<std::string, std::string>& getSustituciones() const;
        void mostrarSustituciones() const;
    };