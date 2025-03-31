#pragma once
#include <string>
#include <vector>
#include <iostream>

class Relacion {
private:
    std::string tipo_relacion;
    std::string nombreFuncion;
    std::vector<std::string> argumentos;
    bool esNegacion;

public:
    Relacion(std::string tipo_relacion, 
             std::string nombreFuncion, 
             const std::vector<std::string>& argumentos,
             bool esNegacion = false);
    
    Relacion();
    ~Relacion();
    
    Relacion* toggleNegacion();
    
    std::string getTipoRelacion() const;
    std::string getNombreFuncion() const;
    const std::vector<std::string>& getArgumentos() const;
    bool getEsNegacion() const;

    void mostrar() const;
};

            



