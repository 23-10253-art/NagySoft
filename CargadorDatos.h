#ifndef CARGADOR_DATOS_H
#define CARGADOR_DATOS_H

#include <vector>
#include <string>
#include "Tipos.h"

// Función que lee el CSV y retorna el vector con las coordenadas
std::vector<Coord_3D> cargarDatos(const std::string& nombreArchivo);

#endif // CARGADOR_DATOS_H