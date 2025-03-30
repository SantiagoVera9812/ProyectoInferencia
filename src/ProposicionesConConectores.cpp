#include "ProposicionesConConectores.h"
#include <vector>
#include "Relacion.h"
#include "manejoDeClausulas.h"


ProposicionesConConectores::ProposicionesConConectores(std::string conector, std::vector<Relacion> relaciones)
: conector(conector), relaciones(relaciones) {}

ProposicionesConConectores::ProposicionesConConectores() : conector(""), relaciones() {}

void ProposicionesConConectores::eliminarRelacion(size_t index) {
    if (index < relaciones.size()) {
        relaciones.erase(relaciones.begin() + index);
    }
}

void ProposicionesConConectores::agregarRelacion(Relacion* nuevaRelacion) {
    if (nuevaRelacion) { 
        relaciones.push_back(*nuevaRelacion); 
    }
}
