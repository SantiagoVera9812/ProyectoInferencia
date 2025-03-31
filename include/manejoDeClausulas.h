#pragma once
#include <vector>
#include <string>
#include "Relacion.h"
#include "ProposicionesConConectores.h"

void listarClausulas(std::vector<std::vector<std::string>> clausulas);
std::vector<std::string> consegirPalabrasAcciones();
Relacion* conseguirTeorema(std::vector<std::string> palabrasAcciones);  
Relacion procesarClausulasLogicas(std::vector<std::string> clausulaRelacion, std::vector<std::string> palabrasAcciones);