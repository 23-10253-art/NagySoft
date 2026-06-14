#ifndef KMEANS_H
#define KMEANS_H

#include <vector>
#include "Tipos.h"

// Función principal que ejecutará el bucle K-Means
std::vector<Cluster> ejecutarKMeans(const std::vector<Coord_3D>& datos, int k);

// Herramientas matemáticas auxiliares
double calcularDistancia(const Coord_3D& p1, const Coord_3D& p2);
double calcularDispersion(const Cluster& c);

// Requisito estricto: Condición de parada encapsulada
bool condicionDeParada(const std::vector<Coord_3D>& centroidesAnteriores, const std::vector<Coord_3D>& centroidesNuevos, int iteracionActual, int maxIteraciones);

#endif // KMEANS_H