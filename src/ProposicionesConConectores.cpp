#include "ProposicionesConConectores.h"
#include <vector>
#include "Relacion.h"
#include "manejoDeClausulas.h"

ProposicionesConConectores::ProposicionesConConectores(std::string conector, std::vector<Relacion> relaciones)
: conector(conector),      
  relaciones(relaciones) {} 


ProposicionesConConectores::ProposicionesConConectores() 
: conector(""),       
  relaciones() {}     

// Elimina una relación por su índice
void ProposicionesConConectores::eliminarRelacion(size_t index) {
    // Verifica que el índice esté dentro del rango válido
    if (index < relaciones.size()) {
        // Elimina la relación en la posición indicada
        relaciones.erase(relaciones.begin() + index);
    }
}

// Agrega una nueva relación a la proposición
void ProposicionesConConectores::agregarRelacion(Relacion* nuevaRelacion) {
    if (nuevaRelacion) { 
        relaciones.push_back(*nuevaRelacion); 
    }
}
