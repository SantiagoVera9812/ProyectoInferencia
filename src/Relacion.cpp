#include "Relacion.h"


Relacion::Relacion(std::string tipo_relacion, 
                   std::string nombreFuncion, 
                   const std::vector<std::string>& argumentos,
                   bool esNegacion)
    : tipo_relacion(tipo_relacion),      
      nombreFuncion(nombreFuncion), 
      argumentos(argumentos),             
      esNegacion(esNegacion) {}          

Relacion::Relacion() 
    : tipo_relacion(""), 
      nombreFuncion(""), 
      esNegacion(false) {}

// Destructor 
Relacion::~Relacion() {}

Relacion* Relacion::toggleNegacion() {
    esNegacion = !esNegacion; 
    return this;              
}

// Devuelve el tipo de relación (predicado, definición, acción)
std::string Relacion::getTipoRelacion() const { return tipo_relacion; }

// Devuelve el nombre del predicado
std::string Relacion::getNombreFuncion() const { return nombreFuncion; }

// Devuelve una referencia constante a la lista de argumentos
const std::vector<std::string>& Relacion::getArgumentos() const { return argumentos; }

// Devuelve el estado de negación (true si está negada)
bool Relacion::getEsNegacion() const { return esNegacion; }

void Relacion::mostrar() const {
    std::cout << "Relacion -> Tipo: " << tipo_relacion
              << ", Funcion: " << nombreFuncion << "(";
              
    // Imprime los argumentos separados por comas
    for (size_t i = 0; i < argumentos.size(); ++i) {
        if (i != 0) std::cout << ", ";  
        std::cout << argumentos[i];
    }
    std::cout << ")";
    
    // Indica si la relación está negada
    if (esNegacion) {
        std::cout << " [NEGADO]";
    }
    std::cout << "\n";  
}

