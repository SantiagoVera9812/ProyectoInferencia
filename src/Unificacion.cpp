#include "Unificacion.h"
#include <algorithm>

Unificacion::Unificacion() {}

//Revisa si la primera letra de una palabra esta en minuscula (Variable o constante)
bool Unificacion::esVariable(const std::string& termino) const {
    return !termino.empty() && islower(termino[0]);
}

bool Unificacion::unificar(const Relacion& a, const Relacion& b) {

    // Comprueba si las relaciones tienen el mismo nombre de función y el mismo número de argumentos
    if (a.getNombreFuncion() != b.getNombreFuncion() || 
        a.getArgumentos().size() != b.getArgumentos().size()) {
        return false; // Si no coinciden, no se pueden unificar
    }

    // Recorremos los argumentos de ambas relaciones

    for (size_t i = 0; i < a.getArgumentos().size(); ++i) {

        // Conseguir un argumento de ambas relaciones
        const std::string& term1 = a.getArgumentos()[i];
        const std::string& term2 = b.getArgumentos()[i];

         // Si el primer término es una variable
        if (esVariable(term1)) {
            auto it = sustituciones.find(term1);
            if (it != sustituciones.end()) {
                // Si la variable ya tiene una sustitución, debe coincidir con el segundo término
                if (it->second != term2) return false;
            } else {
                // Si no tiene sustitución, la agregamos
                sustituciones[term1] = term2;
            }
        }  // Si el segundo término es una variable
        else if (esVariable(term2)) {
            // Si ya tiene una sustitución, debe coincidir con el primer término
            auto it = sustituciones.find(term2);
            if (it != sustituciones.end()) {
                if (it->second != term1) return false;
            } else {
                // Si no tiene sustitución, la agregamos
                sustituciones[term2] = term1;
            }
            // Si ambos términos son constantes y distintos, la unificación falla
        } else {
            if (term1 != term2) return false;
        }
    }
    return true; // Si hemos recorrido todos los argumentos sin conflicto, la unificación es posible
}


//Una relación y le aplica las sustituciones encontradas durante la unificación
Relacion Unificacion::aplicarSustitucion(const Relacion& rel) const {
    std::vector<std::string> nuevosArgumentos;
    // Recorremos los argumentos de la relación
    for (const auto& arg : rel.getArgumentos()) {
        if (esVariable(arg)) {
            auto it = sustituciones.find(arg);
            // Si la variable tiene una sustitución, se reemplaza
            nuevosArgumentos.push_back(it != sustituciones.end() ? it->second : arg);
        } else {
            // Si no es una variable, se deja el argumento tal cual
            nuevosArgumentos.push_back(arg);
        }
    }
      // Retornamos una nueva relación con los argumentos sustituidos
    return Relacion(rel.getTipoRelacion(), rel.getNombreFuncion(), nuevosArgumentos, rel.getEsNegacion());
}

//Fusiona la instancia de Unificacion con las sustituciones de otra Unificacion
void Unificacion::combinar(const Unificacion& otra) {
    for (const auto& [var, val] : otra.sustituciones) {
        sustituciones[var] = val;
    }
}
//Eliminar las sustituciones encontradas
void Unificacion::limpiar() {
    sustituciones.clear();
}
//Obtener sustituciones
const std::unordered_map<std::string, std::string>& Unificacion::getSustituciones() const {
    return sustituciones;
}

void Unificacion::mostrarSustituciones() const {
    std::cout << "Sustituciones actuales:\n";
    for (const auto& [var, val] : sustituciones) {
        std::cout << var << " = " << val << "\n";
    }
}