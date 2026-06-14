#include "KMeans.h"
#include <cmath>
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>

// Calcula la distancia Euclidiana entre dos puntos 3D
double calcularDistancia(const Coord_3D& p1, const Coord_3D& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + 
                     std::pow(p1.y - p2.y, 2) + 
                     std::pow(p1.z - p2.z, 2));
}

/* * CONDICION DE PARADA
 * Parámetros:
 * - centroidesAnteriores: posiciones de los centroides en la iteración previa.
 * - centroidesNuevos: posiciones de los centroides recalculadas en la iteración actual.
 * - iteracionActual: contador de los ciclos ejecutados.
 * - maxIteraciones: límite de seguridad de ciclos.
 * * Explicación de por qué es una buena condición:
 * En el algoritmo de K-Means, el objetivo es que los centroides se estabilicen. 
 * Esta función es ideal porque evalúa si la distancia de movimiento de los centroides 
 * es menor a un umbral mínimo (0.0001); si ya no se mueven, el algoritmo ha convergido 
 * y debe detenerse. Adicionalmente, el parámetro de maxIteraciones actúa como un 
 * mecanismo de seguridad vital para evitar bucles infinitos si los datos causan oscilación.
 */
bool condicionDeParada(const std::vector<Coord_3D>& centroidesAnteriores, const std::vector<Coord_3D>& centroidesNuevos, int iteracionActual, int maxIteraciones) {
    if (iteracionActual >= maxIteraciones) {
        return true;
    }

    double umbralMovimiento = 0.0001; 
    
    for (size_t i = 0; i < centroidesAnteriores.size(); ++i) {
        double distancia = calcularDistancia(centroidesAnteriores[i], centroidesNuevos[i]);
        if (distancia > umbralMovimiento) {
            return false; // Al menos uno se movió, seguimos iterando
        }
    }
    
    return true; // Ninguno se movió significativamente, el algoritmo terminó
}

// Calcula la dispersión del cluster (promedio de la distancia al cuadrado al centroide)
double calcularDispersion(const Cluster& c) {
    if (c.points.empty()) return 0.0;
    
    double suma = 0.0;
    for (const auto& p : c.points) {
        // Elevamos al cuadrado la distancia Euclidiana
        suma += std::pow(calcularDistancia(p, c.centroid), 2);
    }
    return suma / c.points.size();
}

// Función principal que agrupa los datos en 'k' clusters
std::vector<Cluster> ejecutarKMeans(const std::vector<Coord_3D>& datos, int k) {
    std::vector<Cluster> clusters(k);
    std::vector<Coord_3D> centroidesAnteriores(k);
    std::vector<Coord_3D> centroidesNuevos(k);
    
    // Inicialización: Seleccionamos k puntos aleatorios de los datos como centroides iniciales
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int i = 0; i < k; ++i) {
        clusters[i].label = 'A' + i; // Asigna etiquetas A, B, C, etc.
        int randomIndex = std::rand() % datos.size();
        centroidesNuevos[i] = datos[randomIndex];
        clusters[i].centroid = centroidesNuevos[i];
    }

    int iteracionActual = 0;
    int maxIteraciones = 100; // Límite de seguridad

    // El bucle se repite hasta que la condición de parada se cumpla
    while (!condicionDeParada(centroidesAnteriores, centroidesNuevos, iteracionActual, maxIteraciones)) {
        centroidesAnteriores = centroidesNuevos;
        
        // 1. Limpiar los puntos asignados en la iteración anterior
        for (int i = 0; i < k; ++i) {
            clusters[i].points.clear();
        }

        // 2. Asignar cada punto al centroide más cercano
        for (const auto& punto : datos) {
            int indexMejorCluster = 0;
            double distanciaMinima = std::numeric_limits<double>::max();

            for (int i = 0; i < k; ++i) {
                double dist = calcularDistancia(punto, clusters[i].centroid);
                if (dist < distanciaMinima) {
                    distanciaMinima = dist;
                    indexMejorCluster = i;
                }
            }
            clusters[indexMejorCluster].points.push_back(punto);
        }

        // 3. Recalcular la posición de los centroides (promedio de las coordenadas)
        for (int i = 0; i < k; ++i) {
            if (!clusters[i].points.empty()) {
                double sumX = 0, sumY = 0, sumZ = 0;
                for (const auto& p : clusters[i].points) {
                    sumX += p.x;
                    sumY += p.y;
                    sumZ += p.z;
                }
                int numPuntos = clusters[i].points.size();
                centroidesNuevos[i] = {sumX / numPuntos, sumY / numPuntos, sumZ / numPuntos};
                clusters[i].centroid = centroidesNuevos[i];
            } else {
                // Si por alguna razón matemática un cluster se queda vacío, mantiene su posición
                centroidesNuevos[i] = centroidesAnteriores[i];
            }
        }

        iteracionActual++;
    }

    return clusters;
}