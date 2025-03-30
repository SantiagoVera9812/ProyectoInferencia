#include <string>
#include "Relacion.h"
#include <vector>
#pragma once

enum Conector{
    Y, O , SI, ENTONCES
};

class ProposicionesConConectores
{
private:

        std::vector<Relacion> relaciones;
        std::string conector;

    
public:
    
    ProposicionesConConectores(std::string conector, std::vector<Relacion> relaciones);

    ProposicionesConConectores();

    void agregarRelacion(Relacion* nuevaRelacion);

    void eliminarRelacion(size_t index);

    void mostrar() const {
        std::cout << "Proposiciones con conector: " << conector << std::endl;
        for (auto& relacion : relaciones) {
            relacion.mostrar();
        }
    }



    const std::vector<Relacion>& getRelaciones() const {
        return relaciones;
    }

    std::vector<Relacion>& getRelaciones() {
        return relaciones;
    }
};


