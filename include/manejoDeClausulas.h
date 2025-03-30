#include <iostream>
#include <vector>
#include "ProposicionesConConectores.h"
#pragma once


void listarClausulas(std::vector<std::vector<std::string>> clausulas);
std::vector<std::string> consegirPalabrasAcciones();
Relacion conseguirTeorema(std::vector<std::string> palabrasAcciones);
std::vector<ProposicionesConConectores> procesarConectoresLogicos(std::vector<std::vector<std::string>> clausulas, std::vector<std::string> acciones);
Relacion procesarClausulasLogicas(std::vector<std::string> clausulaRelacion, std::vector<std::string> palabrasAcciones);