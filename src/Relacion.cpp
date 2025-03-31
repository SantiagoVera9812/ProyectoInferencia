#include "Relacion.h"

Relacion::Relacion(std::string tipo_relacion, 
                   std::string nombreFuncion, 
                   const std::vector<std::string>& argumentos,
                   bool esNegacion)
    : tipo_relacion(tipo_relacion), 
      nombreFuncion(nombreFuncion), 
      argumentos(argumentos),
      esNegacion(esNegacion) {}

Relacion::Relacion() : tipo_relacion(""), nombreFuncion(""), esNegacion(false) {}

Relacion::~Relacion() {}

Relacion* Relacion::toggleNegacion() {
    esNegacion = !esNegacion;
    return this;
}

std::string Relacion::getTipoRelacion() const { return tipo_relacion; }
std::string Relacion::getNombreFuncion() const { return nombreFuncion; }
const std::vector<std::string>& Relacion::getArgumentos() const { return argumentos; }
bool Relacion::getEsNegacion() const { return esNegacion; }

void Relacion::mostrar() const {
    std::cout << "Relacion -> Tipo: " << tipo_relacion
              << ", Funcion: " << nombreFuncion << "(";
    for (size_t i = 0; i < argumentos.size(); ++i) {
        if (i != 0) std::cout << ", ";
        std::cout << argumentos[i];
    }
    std::cout << ")";
    if (esNegacion) {
        std::cout << " [NEGADO]";
    }
    std::cout << "\n";
}

