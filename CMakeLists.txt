cmake_minimum_required(VERSION 3.10)
project(ProyectoInferencia)

# Especificar el estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Agregar el directorio de encabezados
include_directories(include)

# Agregar los archivos fuente
set(SOURCES
    src/IntroduccionAIaProyecto2.cpp
    src/manejoDeClausulas.cpp
    src/maquinaDeInferencia.cpp
    src/procesarConector.cpp
    src/ProposicionesConConectores.cpp
    src/Relacion.cpp
)

# Crear el ejecutable
add_executable(maquinaDeInferencia ${SOURCES})

